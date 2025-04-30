# -*- coding: utf-8 -*-
#-----------------------------------------------------------------------------#

#      Projet Chiffrement vigener

#----------------------------------------------------------------------------#


# -------------- Importation des modules nécessaires -------------------------#

from tkinter import *
import chiffrement_vigener


# --------------------- Déclaration des variables globales -------------------#


# --------------------- Définition des fonctions -----------------------------#

def page(text1: str, text2: str, fonction):
    """ Génere la page choisie """

    def chiffrement(dechiffre):
        """ Fait le calcule en fonction de la page dans laquelle on se trouve """
        if dechiffre:
            """ Effectue et afficher la réponse du déchiffrement """
            entry3.delete(1.0, END)
            entry3.insert(1.0, chiffrement_vigener.dechiffrer(entry1.get(), entry2.get()))
        else:
            """ Effectue et afficher la réponse du chiffrement """
            entry3.delete(1.0, END)
            entry3.insert(1.0, chiffrement_vigener.chiffrer(entry1.get(), entry2.get()))


    def back():
        """ retourne sur la page précedente """
        text3.pack_forget()
        entry1.pack_forget()
        entry2.pack_forget()
        entry3.pack_forget()
        back.pack_forget()
        valide_btn.pack_forget()
        text2.pack_forget()
        text1.pack_forget()
        button1.pack(fill=X, padx=10, pady=35)
        button2.pack(fill=X, padx=10, pady=35)

    button1.pack_forget()
    button2.pack_forget()

    text3 = Label(frame, text=text1, bg="#24292E", fg="red", font=("Courrier", 30, "bold"))
    text3.pack(pady=5)
    entry1 = Entry(frame, bg="white", fg="#24292E", borderwidth=20, font=("Courrier", 20, "bold"), width=35)
    entry1.pack(pady=20)
    text1 = Label(frame, text=text2, bg="#24292E", fg="red", font=("Courrier", 30, "bold"))
    text1.pack(pady=5)
    entry2 = Entry(frame, bg="white", fg="#24292E", borderwidth=20, font=("Courrier", 20, "bold"))
    entry2.pack(pady=20)
    valide_btn = Button(frame, text="VALIDE", bg="green", fg="black", font=("Courrier", 30, "bold"), borderwidth=25, command=lambda: chiffrement(fonction))
    valide_btn.pack(pady=30)
    text2 = Label(frame, text="Sortie :", bg="#24292E", fg="red", font=("Courrier", 30, "bold"))
    text2.pack(pady=5)
    entry3 = Text(frame, bg="white", fg="#24292E", borderwidth=20, font=("Courrier", 20, "bold"), width=40, height=5)
    entry3.pack(pady=20)
    back = Button(frame, text="Retour", bg="#24292E", fg="white", font=("Courrier", 20, "bold"), borderwidth=10, command=back)
    back.pack(pady=30)


# ----------------- Boucle de Jeu ou programme principal ---------------------#


# Parametrage de la fenetre
window = Tk()
window.title("CHIFFREMENT DE VIGENERE")
window.geometry("1200x800")
window.config(bg="#24292E")
window.iconbitmap("cadena_logo.ico")

# Creation de la frame
frame = Frame(window, bg="#24292E")
frame.pack(expand=YES)

# Mise en place des boutons dans le page d'accueille
button1 = Button(frame, text="Chiffré", bg="green", fg="black", font=("Courrier", 55, "bold"), borderwidth=25, command=lambda: page("Text à chiffré :", "Clé de chiffrement :", False))
button1.pack(fill=X, padx=10, pady=35)
button2 = Button(frame, text="Déchiffre", bg="green", fg="black", font=("Courrier", 55, "bold"), borderwidth=25, command=lambda: page("Text à déchiffré :", "Clé de déchiffrement :", True))
button2.pack(fill=X, padx=10, pady=35)


# Pour faire tourner la fenetre
window.mainloop()