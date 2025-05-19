import random
import networkx as nx
import numpy as np
import matplotlib.pyplot as plt
from matplotlib.lines import Line2D
from dataclasses import dataclass, field
from typing import List, Dict, Optional, Set # Restore necessary typing imports
import time
import sys
import copy

try:
    from lecture_fichier import fichier_to_liste, liste_to_dictionnaire
except ImportError:
    print("ERREUR: 'lecture_fichier.py' n'est pas accessible.")
    sys.exit(1)

BASIC_COLORS = [
    'blue', 'green', 'red', 'cyan', 'magenta', 'yellow', 'black', 'orange', 'purple'
]

@dataclass
class Vehicle:
    id: int
    capacity: float
    current_location: int = 0
    speed: float = 1.0
    color: Optional[str] = None # Use Optional for None default
    assigned_nodes: Set[int] = field(default_factory=set)
    current_load: float = 0.0

    def __post_init__(self):
        if self.color is None:
            self.color = random.choice(BASIC_COLORS)

    def get_adjusted_weight(self, G, u, v):
        if u == v: return 0.0
        try:
            if self.speed <= 0.0: return float('inf')
            if u not in G or v not in G: return float('inf')
            edge_data = G.edges[u, v]
            base_weight = edge_data['weight']
            factor = edge_data.get('traffic_factor', 1.0)
            return (base_weight * factor) / self.speed
        except KeyError:
            return float('inf')
        except Exception:
             return float('inf')

    def can_carry(self, demand):
        return self.current_load + demand <= self.capacity + 1e-9

    def load(self, demand):
        self.current_load += demand

    def unload(self):
        self.current_load = 0.0

class MultiVehicleOptimizer:

    def __init__(self, data_file = None):
        self.G = nx.Graph() # Type hint removed but nx.Graph() initializes
        self.vehicles: List[Vehicle] = []
        self.pos: Dict = {}
        self.depot_node: int = 0
        self.max_quantity: float = 0.0
        self.client_data_by_id: Dict = {}
        self.num_depots: int = 0
        self.num_clients: int = 0
        self.node_demands: Dict = {}
        self.edge_weights_cache: Dict = {}
        if data_file:
            try:
                self.load_data_from_file(data_file)
            except (FileNotFoundError, ValueError, Exception) as e:
                print(f"ERREUR Chargement: {e}")
                raise

    def _get_edge_weight(self, vehicle, u, v):
        # Type hints removed from arguments vehicle, u, v
        key = (vehicle.id, u, v) if u <= v else (vehicle.id, v, u)
        if key not in self.edge_weights_cache:
             weight = vehicle.get_adjusted_weight(self.G, u, v)
             self.edge_weights_cache[key] = weight
        return self.edge_weights_cache.get(key, float('inf'))

    def load_data_from_file(self, data_file):
        print(f"Chargement depuis : {data_file}")
        lines = fichier_to_liste(data_file)
        data = liste_to_dictionnaire(lines)
        if data is None: raise ValueError("Analyse du fichier échouée.")

        self.num_depots = int(data.get('nb_depots', 0))
        self.num_clients = int(data.get('nb_clients', 0))
        self.max_quantity = float(data.get('max_quantity', 0.0))
        if self.num_depots <= 0 or self.num_clients <= 0 or self.max_quantity <= 0:
             raise ValueError("Données NB_DEPOTS, NB_CLIENTS ou MAX_QUANTITY invalides ou nulles.")
        print(f"  - Dépôts lus: {self.num_depots}, Clients: {self.num_clients}, Capacité/Véhicule: {self.max_quantity}")
        if self.num_depots > 1:
             print(f"Avertissement: Ce code gère un seul dépôt (le premier de la liste). {self.num_depots} dépôts lus.")

        depot_data = data.get('depots', {})
        if not depot_data : raise ValueError("Données dépôt manquantes.")
        depot_id_str = next(iter(depot_data))
        depot_info = depot_data[depot_id_str]
        if 'x' not in depot_info or 'y' not in depot_info: raise ValueError("Coordonnées dépôt manquantes.")
        depot_x, depot_y = float(depot_info['x']), float(depot_info['y'])
        self.G.add_node(self.depot_node, pos=(depot_x, depot_y), type='depot', demand=0.0)
        self.pos[self.depot_node] = (depot_x, depot_y)
        self.node_demands[self.depot_node] = 0.0

        client_raw_data = data.get('clients', {})
        actual_clients_loaded: int = 0
        client_nodes_map: Dict = {}
        current_node_id: int = 1

        try:
            sorted_client_keys = sorted(client_raw_data.keys(), key=lambda x: int(x.replace('c', '')))
        except ValueError:
             print("Avertissement: Tri numérique des IDs clients échoué. Utilisation de l'ordre du fichier.")
             sorted_client_keys = list(client_raw_data.keys())

        for client_id_str in sorted_client_keys:
            client = client_raw_data[client_id_str]
            if 'x' not in client or 'y' not in client or 'demand' not in client:
                 print(f"Avertissement: Données manquantes pour client {client_id_str}. Ignoré.")
                 continue

            client_x, client_y = float(client['x']), float(client['y'])
            client_demand = float(client['demand'])
            if client_demand < 0: client_demand = 0.0

            internal_id = current_node_id
            self.G.add_node(internal_id, pos=(client_x, client_y), type='client', demand=client_demand, original_id=client_id_str)
            self.pos[internal_id] = (client_x, client_y)
            self.client_data_by_id[internal_id] = client
            self.node_demands[internal_id] = client_demand
            client_nodes_map[client_id_str] = internal_id
            actual_clients_loaded += 1
            current_node_id += 1

        if actual_clients_loaded != self.num_clients:
             print(f"Avertissement: Nb clients chargés ({actual_clients_loaded}) != NB_CLIENTS ({self.num_clients}). Utilisation du nombre réel.")
             self.num_clients = actual_clients_loaded

        nodes = list(self.G.nodes)
        for i in range(len(nodes)):
            for j in range(i + 1, len(nodes)):
                u, v = nodes[i], nodes[j]
                pos_u = self.G.nodes[u]['pos']
                pos_v = self.G.nodes[v]['pos']
                dist = np.hypot(pos_u[0] - pos_v[0], pos_u[1] - pos_v[1])
                dist = max(dist, 1e-9)
                self.G.add_edge(u, v, weight=dist)
        print("Chargement terminé.")

    def add_vehicle(self):
        if self.max_quantity <= 0: raise ValueError("Capacité véhicule invalide.")
        # Need to specify the type for dataclass Vehicle
        vehicle = Vehicle(id=len(self.vehicles), capacity=self.max_quantity)
        self.vehicles.append(vehicle)
        self.edge_weights_cache.clear()
        return vehicle

    def calculate_path_cost(self, path, vehicle):
        if not path or len(path) <= 1: return 0.0
        cost: float = 0.0
        for i in range(len(path) - 1):
            u, v = path[i], path[i+1]
            edge_weight = self._get_edge_weight(vehicle, u, v)
            if edge_weight == float('inf'):
                return float('inf')
            cost += edge_weight
        return cost

    def calculate_total_cost(self, paths):
         total_cost: float = 0.0
         vehicle_map: Dict[int, Vehicle] = {v.id: v for v in self.vehicles}
         for vid, path in paths.items():
             vehicle_obj = vehicle_map.get(vid)
             if vehicle_obj:
                 cost = self.calculate_path_cost(path, vehicle_obj)
                 if cost == float('inf'): return float('inf')
                 total_cost += cost
             else:
                 return float('inf')
         return total_cost

    def check_path_capacity(self, path, vehicle):
        simulated_load: float = 0.0
        capacity: float = vehicle.capacity + 1e-9
        if not path: return True
        if len(path) > 1 and (path[0] != self.depot_node or path[-1] != self.depot_node):
            return False

        depot_visits: int = 0
        for i, node in enumerate(path):
            if node == self.depot_node:
                depot_visits += 1
                simulated_load = 0.0
                if depot_visits > 1 and i != len(path) - 1:
                    return False
            else:
                demand = self.node_demands.get(node, float('inf'))
                if demand == float('inf'):
                    return False
                if simulated_load + demand <= capacity:
                    simulated_load += demand
                else:
                    return False

        if len(path) > 1 and path[-1] != self.depot_node:
             return False

        return True

    def build_initial_routes_adaptive(self):
        print("\nGénération adaptative des chemins initiaux (Nearest Neighbor - Single Trip)...")
        initial_paths: Dict[int, List[int]] = {}
        unvisited_clients: Set[int] = set(n for n in self.G.nodes if self.G.nodes[n]['type'] == 'client')
        self.vehicles = []
        self.edge_weights_cache.clear()

        while unvisited_clients:
            vehicle = self.add_vehicle()
            current_path: List[int] = [self.depot_node]
            current_node: int = self.depot_node
            current_load: float = 0.0
            served_in_this_trip: Set[int] = set()

            while True:
                best_next_node: Optional[int] = None
                min_dist: float = float('inf')

                for node in unvisited_clients:
                     demand = self.node_demands.get(node, float('inf'))
                     if current_load + demand <= vehicle.capacity + 1e-9 :
                         dist = self._get_edge_weight(vehicle, current_node, node)
                         if dist < min_dist:
                             min_dist = dist
                             best_next_node = node

                if best_next_node is not None:
                    demand = self.node_demands[best_next_node]
                    current_load += demand
                    current_path.append(best_next_node)
                    current_node = best_next_node
                    served_in_this_trip.add(best_next_node)
                    unvisited_clients.remove(best_next_node)
                else:
                    break

            if len(current_path) > 1:
                current_path.append(self.depot_node)
                vehicle.assigned_nodes = served_in_this_trip
                initial_paths[vehicle.id] = current_path
            else:
                self.vehicles.pop()
                if not unvisited_clients:
                    break
                else:
                    unservable_clients = {c for c in unvisited_clients if self.node_demands.get(c, 0) > vehicle.capacity}
                    if unservable_clients:
                         print(f"ERREUR: Impossible de servir les clients {unservable_clients} car leur demande dépasse la capacité du véhicule ({vehicle.capacity}).")
                    else:
                         print(f"ERREUR: Clients restants {unvisited_clients} mais impossible de créer une nouvelle route.")
                    raise RuntimeError("Échec de l'assignation de tous les clients aux routes initiales.")

        print(f"Nombre final de véhicules nécessaires: {len(self.vehicles)}")
        self.edge_weights_cache.clear()
        for v in self.vehicles:
             for u_node in self.G.nodes:
                 for v_node in self.G.nodes:
                      if u_node < v_node: self._get_edge_weight(v, u_node, v_node)

        return initial_paths

    def calculate_delta_cost_intra_exchange(self, vehicle, path, idx1, idx2):
        n1, n2 = path[idx1], path[idx2]
        if idx1 <= 0 or idx1 >= len(path)-1 or idx2 <= 0 or idx2 >= len(path)-1: return float('inf')
        if abs(idx1 - idx2) == 0 : return 0.0

        prev1, next1 = path[idx1-1], path[idx1+1]
        prev2, next2 = path[idx2-1], path[idx2+1]

        if abs(idx1 - idx2) == 1:
             idx_min = min(idx1, idx2); idx_max = max(idx1, idx2)
             node_min, node_max = path[idx_min], path[idx_max]
             prev_min = path[idx_min-1]
             next_max = path[idx_max+1]

             current_adj_cost = (self._get_edge_weight(vehicle, prev_min, node_min) +
                                 self._get_edge_weight(vehicle, node_min, node_max) +
                                 self._get_edge_weight(vehicle, node_max, next_max))
             new_adj_cost = (self._get_edge_weight(vehicle, prev_min, node_max) +
                             self._get_edge_weight(vehicle, node_max, node_min) +
                             self._get_edge_weight(vehicle, node_min, next_max))
             if current_adj_cost == float('inf') or new_adj_cost == float('inf'): return float('inf')
             return new_adj_cost - current_adj_cost
        else:
             current_cost_seg1 = self._get_edge_weight(vehicle, prev1, n1) + self._get_edge_weight(vehicle, n1, next1)
             current_cost_seg2 = self._get_edge_weight(vehicle, prev2, n2) + self._get_edge_weight(vehicle, n2, next2)
             new_cost_seg1 = self._get_edge_weight(vehicle, prev1, n2) + self._get_edge_weight(vehicle, n2, next1)
             new_cost_seg2 = self._get_edge_weight(vehicle, prev2, n1) + self._get_edge_weight(vehicle, n1, next2)
             if current_cost_seg1 == float('inf') or current_cost_seg2 == float('inf') or \
                new_cost_seg1 == float('inf') or new_cost_seg2 == float('inf'):
                 return float('inf')
             return (new_cost_seg1 + new_cost_seg2) - (current_cost_seg1 + current_cost_seg2)

    def calculate_delta_cost_intra_relocate(self, vehicle, path, idx_remove, idx_insert):
         if idx_remove <= 0 or idx_remove >= len(path)-1 or idx_insert <= 0 or idx_insert > len(path)-1:
              return float('inf')

         node_to_move = path[idx_remove]
         prev_rem = path[idx_remove-1]
         next_rem = path[idx_remove+1]

         cost_removed = (self._get_edge_weight(vehicle, prev_rem, node_to_move) +
                         self._get_edge_weight(vehicle, node_to_move, next_rem) -
                         self._get_edge_weight(vehicle, prev_rem, next_rem))

         if idx_insert == idx_remove or idx_insert == idx_remove + 1:
              if idx_insert == idx_remove:
                  prev_ins = path[idx_insert-1]
                  next_ins = path[idx_insert]
                  cost_added = (self._get_edge_weight(vehicle, prev_ins, node_to_move) +
                                self._get_edge_weight(vehicle, node_to_move, next_ins) -
                                self._get_edge_weight(vehicle, prev_ins, next_ins))
              else:
                   prev_ins = path[idx_remove]
                   next_ins = path[idx_insert]
                   cost_added = (self._get_edge_weight(vehicle, prev_ins, node_to_move) +
                                 self._get_edge_weight(vehicle, node_to_move, next_ins) -
                                 self._get_edge_weight(vehicle, prev_ins, next_ins))
         elif idx_insert < idx_remove:
              prev_ins = path[idx_insert-1]
              next_ins = path[idx_insert]
              cost_added = (self._get_edge_weight(vehicle, prev_ins, node_to_move) +
                            self._get_edge_weight(vehicle, node_to_move, next_ins) -
                            self._get_edge_weight(vehicle, prev_ins, next_ins))
         else:
              prev_ins = path[idx_insert-1]
              next_ins = path[idx_insert] if idx_insert < len(path) else path[-1]
              cost_added = (self._get_edge_weight(vehicle, prev_ins, node_to_move) +
                             self._get_edge_weight(vehicle, node_to_move, next_ins) -
                             self._get_edge_weight(vehicle, prev_ins, next_ins))

         if cost_removed == float('inf') or cost_added == float('inf'):
             return float('inf')

         return cost_added - cost_removed

    def calculate_delta_cost_inter_exchange(self, v1, path1, idx1, v2, path2, idx2):
         if idx1 <= 0 or idx1 >= len(path1)-1 or idx2 <= 0 or idx2 >= len(path2)-1: return float('inf')

         node1 = path1[idx1]
         node2 = path2[idx2]
         prev1, next1 = path1[idx1-1], path1[idx1+1]
         prev2, next2 = path2[idx2-1], path2[idx2+1]

         current_cost1 = self._get_edge_weight(v1, prev1, node1) + self._get_edge_weight(v1, node1, next1)
         current_cost2 = self._get_edge_weight(v2, prev2, node2) + self._get_edge_weight(v2, node2, next2)

         new_cost1 = self._get_edge_weight(v1, prev1, node2) + self._get_edge_weight(v1, node2, next1)
         new_cost2 = self._get_edge_weight(v2, prev2, node1) + self._get_edge_weight(v2, node1, next2)

         if current_cost1 == float('inf') or current_cost2 == float('inf') or \
            new_cost1 == float('inf') or new_cost2 == float('inf'):
              return float('inf')

         return (new_cost1 + new_cost2) - (current_cost1 + current_cost2)

    def simulated_annealing_global(self, initial_paths,
                                   initial_temperature = 100.0, cooling_rate = 0.99,
                                   min_temperature = 0.1, iterations_per_temp = 500,
                                   max_time_seconds = 30.0,
                                   force_duration = False):

        sa_start_time = time.time()
        mode_str = "(Mode Forcé)" if force_duration else ""
        print(f"\nLancement du Recuit Simulé Global {mode_str} (Tmax={max_time_seconds:.1f}s)...")

        current_paths: Dict[int, List[int]] = {vid: list(path) for vid, path in initial_paths.items()}
        best_paths: Dict[int, List[int]] = copy.deepcopy(initial_paths)

        current_total_cost: float = self.calculate_total_cost(current_paths)
        if current_total_cost == float('inf'):
            print("ERREUR CRITIQUE: Coût initial SA Global infini.")
            return initial_paths

        best_total_cost: float = current_total_cost
        temperature: float = initial_temperature
        total_iterations: int = 0

        active_vehicle_ids: List[int] = [vid for vid, path in current_paths.items() if len(path) > 2]
        num_active_vehicles: int = len(active_vehicle_ids)

        op_weights: Dict[str, float] = {'intra_exchange': 0.4, 'intra_relocate': 0.4, 'inter_exchange': 0.2}
        op_types: List[str] = list(op_weights.keys())
        op_probs: List[float] = list(op_weights.values())

        very_low_temp_threshold: float = min_temperature * 0.01

        while True:
            time_elapsed = time.time() - sa_start_time
            if max_time_seconds and time_elapsed >= max_time_seconds:
                print(f"  Limite de temps SA atteinte ({time_elapsed:.1f}s).")
                break

            if not force_duration and temperature <= min_temperature:
                 print(f"  Température minimale atteinte (mode normal).")
                 break

            effective_cooling_rate = cooling_rate
            if force_duration and temperature <= very_low_temp_threshold:
                effective_cooling_rate = 1.0

            accepted_moves_in_batch: int = 0
            for _ in range(iterations_per_temp):
                if max_time_seconds and (time.time() - sa_start_time) >= max_time_seconds:
                    force_duration = False
                    min_temperature = float('inf')
                    break

                total_iterations += 1
                if not active_vehicle_ids: break

                num_active_vehicles = len(active_vehicle_ids)
                if num_active_vehicles == 0: break

                current_op_types = op_types
                current_op_probs = op_probs
                if num_active_vehicles < 2:
                     non_inter_weights = [op_weights[op] for op in op_types if op != 'inter_exchange']
                     total_weight = sum(non_inter_weights)
                     current_op_types = [op for op in op_types if op != 'inter_exchange']
                     current_op_probs = [w / total_weight for w in non_inter_weights] if total_weight > 0 else []
                     if not current_op_types: continue

                op_choice = random.choices(current_op_types, weights=current_op_probs, k=1)[0]

                cost_diff: float = float('inf')
                move_details = None
                temp_path_info: Dict = {}

                try:
                    if op_choice == 'intra_exchange' or op_choice == 'intra_relocate':
                        if not active_vehicle_ids: continue
                        v_id = random.choice(active_vehicle_ids)
                        path = current_paths[v_id]
                        num_clients = len(path) - 2

                        if num_clients >= 2:
                             idx1 = random.randint(1, num_clients)
                             vehicle = self.vehicles[v_id]

                             if op_choice == 'intra_exchange':
                                 idx2 = random.randint(1, num_clients)
                                 while idx1 == idx2: idx2 = random.randint(1, num_clients)
                                 cost_diff = self.calculate_delta_cost_intra_exchange(vehicle, path, min(idx1, idx2), max(idx1, idx2))
                                 if cost_diff != float('inf'):
                                      move_details = ('intra_exchange', v_id, idx1, idx2)

                             else:
                                 idx_remove = idx1
                                 if len(path) <= 3: continue

                                 temp_path = path[:idx_remove] + path[idx_remove+1:]
                                 idx_insert = random.randint(1, len(temp_path) - 1)
                                 node_to_move = path[idx_remove]
                                 temp_path.insert(idx_insert, node_to_move)

                                 if self.check_path_capacity(temp_path, vehicle):
                                      cost_diff = self.calculate_delta_cost_intra_relocate(vehicle, path, idx_remove, idx_insert)
                                      if cost_diff != float('inf'):
                                           move_details = ('intra_relocate', v_id)
                                           temp_path_info[v_id] = temp_path


                    elif op_choice == 'inter_exchange' and num_active_vehicles >= 2:
                        v_id1, v_id2 = random.sample(active_vehicle_ids, 2)
                        path1 = current_paths[v_id1]
                        path2 = current_paths[v_id2]
                        num_clients1 = len(path1) - 2
                        num_clients2 = len(path2) - 2

                        if num_clients1 >= 1 and num_clients2 >= 1:
                             idx1 = random.randint(1, num_clients1)
                             idx2 = random.randint(1, num_clients2)
                             vehicle1 = self.vehicles[v_id1]
                             vehicle2 = self.vehicles[v_id2]

                             temp_path1 = list(path1); temp_path2 = list(path2)
                             temp_path1[idx1], temp_path2[idx2] = temp_path2[idx2], temp_path1[idx1]

                             if self.check_path_capacity(temp_path1, vehicle1) and self.check_path_capacity(temp_path2, vehicle2):
                                 cost_diff = self.calculate_delta_cost_inter_exchange(vehicle1, path1, idx1, vehicle2, path2, idx2)
                                 if cost_diff != float('inf'):
                                      move_details = ('inter_exchange', v_id1, v_id2)
                                      temp_path_info[v_id1] = temp_path1
                                      temp_path_info[v_id2] = temp_path2

                except IndexError: pass
                except KeyError: pass

                if move_details and cost_diff != float('inf'):
                    acceptance_probability = np.exp(-cost_diff / temperature) if cost_diff > 0 else 1.0

                    if cost_diff < 0 or random.random() < acceptance_probability:
                        accepted_moves_in_batch += 1
                        op_type = move_details[0]
                        cost_change_recalc: float = 0.0

                        try:
                            if op_type == 'intra_exchange':
                                v_id, i1, i2 = move_details[1], move_details[2], move_details[3]
                                vehicle = self.vehicles[v_id]
                                old_cost = self.calculate_path_cost(current_paths[v_id], vehicle)
                                current_paths[v_id][i1], current_paths[v_id][i2] = current_paths[v_id][i2], current_paths[v_id][i1]
                                new_cost = self.calculate_path_cost(current_paths[v_id], vehicle)
                                if old_cost != float('inf') and new_cost != float('inf'):
                                    cost_change_recalc = new_cost - old_cost
                                else: cost_change_recalc = float('inf')

                            elif op_type == 'intra_relocate':
                                v_id = move_details[1]
                                vehicle = self.vehicles[v_id]
                                old_cost = self.calculate_path_cost(current_paths[v_id], vehicle)
                                current_paths[v_id] = temp_path_info[v_id]
                                new_cost = self.calculate_path_cost(current_paths[v_id], vehicle)
                                if old_cost != float('inf') and new_cost != float('inf'):
                                    cost_change_recalc = new_cost - old_cost
                                else: cost_change_recalc = float('inf')


                            elif op_type == 'inter_exchange':
                                v_id1, v_id2 = move_details[1], move_details[2]
                                vehicle1 = self.vehicles[v_id1]; vehicle2 = self.vehicles[v_id2]
                                old_cost1 = self.calculate_path_cost(current_paths[v_id1], vehicle1)
                                old_cost2 = self.calculate_path_cost(current_paths[v_id2], vehicle2)
                                current_paths[v_id1] = temp_path_info[v_id1]
                                current_paths[v_id2] = temp_path_info[v_id2]
                                new_cost1 = self.calculate_path_cost(current_paths[v_id1], vehicle1)
                                new_cost2 = self.calculate_path_cost(current_paths[v_id2], vehicle2)
                                if old_cost1 != float('inf') and old_cost2 != float('inf') and \
                                   new_cost1 != float('inf') and new_cost2 != float('inf'):
                                    cost_change_recalc = (new_cost1 + new_cost2) - (old_cost1 + old_cost2)
                                else: cost_change_recalc = float('inf')

                            if cost_change_recalc != float('inf'):
                                 current_total_cost += cost_change_recalc
                            else:
                                 current_total_cost = self.calculate_total_cost(current_paths)

                            if current_total_cost < best_total_cost - 1e-9:
                                best_paths = copy.deepcopy(current_paths)
                                best_total_cost = current_total_cost

                        except (KeyError, IndexError):
                            current_total_cost = self.calculate_total_cost(current_paths)

            temperature *= effective_cooling_rate
            if force_duration and temperature < very_low_temp_threshold:
                temperature = very_low_temp_threshold

        final_best_cost = self.calculate_total_cost(best_paths)
        if abs(final_best_cost - best_total_cost) > 1e-5 :
             print(f"Avertissement: Coût recalculé final ({final_best_cost:.2f}) diffère du coût suivi ({best_total_cost:.2f}). Utilisation du coût recalculé.")
             best_total_cost = final_best_cost

        print(f"Fin SA Global. Meilleur coût total trouvé: {best_total_cost:.2f}")
        print(f"  Temps écoulé: {time.time() - sa_start_time:.2f} secondes, Itérations: {total_iterations}")
        return best_paths

    def optimize_routes(self, sa_time_limit_base = 2.0, super_mode = False):
        print("\n=== Optimisation des routes ===")
        if self.num_clients == 0:
            print("Aucun client à servir.")
            return {}

        initial_vehicle_paths = self.build_initial_routes_adaptive()

        if not self.vehicles or not initial_vehicle_paths :
            print("Aucun véhicule n'a été assigné ou aucune route initiale créée.")
            return {}

        initial_total_cost = self.calculate_total_cost(initial_vehicle_paths)

        print(f"\nCoût total initial (NN Adaptatif): {initial_total_cost:.2f} pour {len(self.vehicles)} véhicules")
        self.visualize_all_paths(initial_vehicle_paths,
                                 title=f"Chemins Initiaux (NN Adaptatif) - {len(self.vehicles)} Véhicules") # Removed apply_darkening

        force_sa_duration: bool = False
        if super_mode:
             print("\nMode Super Optimisation Activé!")
             sa_total_time = 120.0
             sa_cooling_rate = 0.995
             sa_iter_per_temp = max(500, 250 * len(self.vehicles))
             force_sa_duration = True
        else:
             sa_total_time = max(15.0, len(self.vehicles) * sa_time_limit_base)
             sa_cooling_rate = 0.99
             sa_iter_per_temp = max(300, 150 * len(self.vehicles))

        optimized_paths = self.simulated_annealing_global(
            initial_paths=initial_vehicle_paths,
            initial_temperature=100.0,
            cooling_rate=sa_cooling_rate,
            min_temperature=0.1,
            iterations_per_temp=sa_iter_per_temp,
            max_time_seconds=sa_total_time,
            force_duration=force_sa_duration
        )

        optimized_total_cost = self.calculate_total_cost(optimized_paths)
        print(f"\nCoût total optimisé (SA Global): {optimized_total_cost:.2f}")

        self.visualize_all_paths(optimized_paths,
                                 title=f"Chemins Optimisés (SA Global) - {len(self.vehicles)} Véhicules") # Removed apply_darkening

        return optimized_paths

    # Removed darken_color static method

    def visualize_all_paths(self, vehicle_paths_to_draw,
                            title = "Visualisation des Tournées"): # Removed apply_darkening parameter
        print(f"Génération visualisation: '{title}'")
        if not vehicle_paths_to_draw:
            print("Aucun chemin à visualiser.")
            return

        path_vids = set(vehicle_paths_to_draw.keys())
        current_vids = {v.id for v in self.vehicles}
        if path_vids != current_vids:
            temp_vehicles = {v.id: v for v in self.vehicles}
            new_vehicle_list = []
            max_existing_id = -1
            if self.vehicles: max_existing_id = max(v.id for v in self.vehicles)

            for vid in sorted(list(path_vids)):
                 if vid in temp_vehicles:
                     new_vehicle_list.append(temp_vehicles[vid])
                 else:
                     max_existing_id = max(vid, max_existing_id + 1)
                     new_v = Vehicle(id=max_existing_id, capacity=self.max_quantity)
                     new_vehicle_list.append(new_v)
            self.vehicles = new_vehicle_list


        fig, ax = plt.subplots(figsize=(14, 12))

        client_nodes = [n for n, data in self.G.nodes(data=True) if data['type'] == 'client']
        nx.draw_networkx_nodes(self.G, self.pos, nodelist=client_nodes, node_color='skyblue', node_size=250, alpha=0.9, ax=ax)
        nx.draw_networkx_nodes(self.G, self.pos, nodelist=[self.depot_node], node_color='red', node_size=400, node_shape='s', ax=ax)
        labels = {n: str(n) for n in self.G.nodes}
        nx.draw_networkx_labels(self.G, self.pos, labels=labels, font_size=7, font_weight='bold', ax=ax)

        legend_handles = []
        vehicle_labels = []

        vehicle_map = {v.id: v for v in self.vehicles}

        for vid, path in vehicle_paths_to_draw.items():
            if not path or len(path) <= 1: continue
            vehicle = vehicle_map.get(vid)
            if not vehicle:
                 continue

            base_color_hex = vehicle.color # Use the simplified color name directly
            path_cost = self.calculate_path_cost(path, vehicle)
            label = f"V{vid} (Coût: {path_cost:.1f})"
            legend_handles.append(Line2D([0], [0], color=base_color_hex, lw=2))
            vehicle_labels.append(label)

            current_color_hex = base_color_hex # No darkening applied
            path_edges = list(zip(path[:-1], path[1:]))

            nx.draw_networkx_edges(self.G, self.pos, edgelist=path_edges,
                                    edge_color=current_color_hex, width=1.8,
                                    style='solid', alpha=0.8, arrows=True,
                                    arrowstyle='-|>', arrowsize=10, node_size=250, ax=ax)

        ax.set_title(title, fontsize=15)
        ax.set_xlabel("Coordonnée X", fontsize=12)
        ax.set_ylabel("Coordonnée Y", fontsize=12)
        if legend_handles:
             ax.legend(handles=legend_handles, labels=vehicle_labels, loc='best', fontsize=9, title="Véhicules", title_fontsize='10')
        ax.grid(True, linestyle=':', alpha=0.6)
        fig.tight_layout()
        ax.set_aspect('equal', adjustable='box')


# ================================ MAIN ================================== #

if __name__ == "__main__":
    start_total_time: float = time.time()
    super_mode: bool = False
    dataset_id: Optional[str] = None

    if len(sys.argv) == 3 and sys.argv[1].lower() == "super":
        super_mode = True
        dataset_id = sys.argv[2]
        print("# Mode Super Optimisation VRP")
    elif len(sys.argv) == 2:
        super_mode = False
        dataset_id = sys.argv[1]
        print("# Optimisation VRP Standard")
    else:
        print("\nERREUR: Arguments invalides.")
        print("Usage Standard: python main.py <numero_dataset>")
        print("Usage SuperOpt: python main.py super <numero_dataset>")
        print("Exemple: python main.py 101")
        print("Exemple: python main.py super 101")
        sys.exit(1)

    data_file: str = f"data/data{dataset_id}.vrp"
    print(f"Utilisation du fichier de données spécifié : {data_file}")

    try:
        # Type hint removed from optimizer variable
        optimizer = MultiVehicleOptimizer(data_file=data_file)
    except Exception as e:
        print(f"\nERREUR FATALE Initialisation: {e}")
        import traceback
        traceback.print_exc()
        sys.exit(1)

    try:
        optimized_paths = optimizer.optimize_routes(
            sa_time_limit_base=2.0,
            super_mode=super_mode
            )
    except Exception as e:
        print(f"\nERREUR FATALE pendant l'optimisation: {e}")
        import traceback
        traceback.print_exc()
        sys.exit(1)

    print("\n=== Résumé Final (Chemins Optimisés par SA Global) ===")
    if not optimized_paths:
         print("Aucun chemin optimisé n'a été généré.")
    else:
        final_total_cost = optimizer.calculate_total_cost(optimized_paths)

        print(f"\nCoût total final optimisé : {final_total_cost:.2f} pour {len(optimizer.vehicles)} véhicules.")

        sorted_paths = sorted(optimized_paths.items())
        vehicle_map_final = {v.id: v for v in optimizer.vehicles}

        for vid, path in sorted_paths:
             vehicle_obj = vehicle_map_final.get(vid)
             if vehicle_obj:
                 cost = optimizer.calculate_path_cost(path, vehicle_obj)
                 print(f"Véhicule {vid}: Coût={cost:.2f}", end="")
                 if len(path) < 40: print(f", Chemin={path}")
                 else: print(f", Chemin=[{path[0]}...{path[-1]}] ({len(path)} étapes)")
             else:
                 print(f"ERREUR: ID véhicule {vid} introuvable lors de l'affichage final.")

    print(f"\nTemps d'exécution total : {time.time() - start_total_time:.2f} secondes")

    if plt.get_fignums():
        print("\nFermez les fenêtres graphiques pour terminer.")
        plt.show(block=True)
    else:
        print("\nFin du script (pas de visualisation affichée).")