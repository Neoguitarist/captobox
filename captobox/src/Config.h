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
	/** Fr�quence des prises de mesures.
	 * Il s'agit en r�alit� du temps entre chaque mesure (en millisecondes)... */
	String frequency;
	/** Vrai si le Wi-Fi doit �tre lanc� en mode client, faux pour le lancer en mode serveur (par d�faut). */
	String client_mode;
	/** SSID � utiliser en mode client. */
	String ssid_client;
	/** Mot de passe � utiliser en mode client. */
	String password_client;
	/** Requ�te (???). */
	String request;
	/** Vrai pour activer le flux, faux sinon (???). */
	String stream;
	/** Nom de l'h�te (???). */
	String host;

	Config();

	/**
	 * Lit et charge les param�tres de configuration depuis le flux donn�.
	 *
	 * @param {input} Flux d�crivant une configuration.
	 */
	void load(Stream& input);
	/**
	 * Sauvegarde cette configuration sur le flux donn�.
	 *
	 * @param {output} Flux sur lequel �crire.
	 */
	void save(Stream& output) const;
	/**
	 * �crit cette configuration sur le flux donn�, dans un format lisible pour l'utilisateur.
	 *
	 * @param {output} Flux sur lequel �crire.
	 */
	void print(Stream& output) const;
};
