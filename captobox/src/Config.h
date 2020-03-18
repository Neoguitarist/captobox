#pragma once

#include <string>
#include <WString.h>
#include <Stream.h>

/**
 * Configuration du programme.
 */
class Config
{
public:

	/** Nom de la mesure 1 choisie. */
	std::string capt1;
	/** Nom de la mesure 2 choisie. */
	std::string capt2;
	/** Fréquence des prises de mesures.
	 * Il s'agit en réalité du temps entre chaque mesure (en millisecondes)... */
	String frequency;
	/** Vrai si le Wi-Fi doit être lancé en mode client, faux pour le lancer en mode serveur (par défaut). */
	String client_mode;
	/** SSID à utiliser en mode client. */
	String ssid_client;
	/** Mot de passe à utiliser en mode client. */
	String password_client;
	/** Requête (???). */
	String request;
	/** Vrai pour activer le flux, faux sinon (???). */
	String stream;
	/** Nom de l'hôte (???). */
	String host;

	Config();

	/**
	 * Lit et charge les paramètres de configuration depuis le flux donné.
	 *
	 * @param {input} Flux décrivant une configuration.
	 */
	void load(Stream& input);
	/**
	 * Sauvegarde cette configuration sur le flux donné.
	 *
	 * @param {output} Flux sur lequel écrire.
	 */
	void save(Stream& output) const;
	/**
	 * Écrit cette configuration sur le flux donné, dans un format lisible pour l'utilisateur.
	 *
	 * @param {output} Flux sur lequel écrire.
	 */
	void print(Stream& output) const;
};
