/** URL à appeler pour récupérer la configuration enregistrée sur le serveur. */
const URL_LOAD_CONFIG = "/lecture_config";
/** URL à appeler pour sauvegarder la configuration sur le serveur. */
const URL_SAVE_CONFIG = "write";

/** Configuration manipulée. */
var config = null;

/**
 * Initialise la vue de configuration de la Captobox.
 */
function init()
{
    config =
    {
        capt1: new Param(new InputRef("liste1"), "capt1"),
        capt2: new Param(new InputRef("liste2"), "capt2"),
        frequency: new Param(new InputRef("frequency"), "frequency", 2000),
        client: new Param(new CheckboxRef("client"), "client"),
        request: new Param(new InputRef("request"), "request"),
        host: new Param(new InputRef("host"), "host"),
        ssid: new Param(new InputRef("ssid"), "ssid"),
        password: new Param(new InputRef("password"), "password"),
        stream: new Param(new CheckboxRef("stream"), "stream"),
        version: new Param(new BlockContentRef("p1"), "vers")
    };

    //load_config();
}

function redirect_to(url)
{
    console.log("[config] redirection vers: " + url);
    window.location = url;
}

/**
 * Charge la configuration enregistrée sur le serveur.
 */
function load_config()
{
    /* Envoi une requête pour récupérer la configuration enregistrée sur le serveur. */
    call_url(URL_LOAD_CONFIG,
        function (responseText)
        {
            let jsonData = JSON.parse(responseText);

            /* Chargement de la configuration reçue. */
            Object.keys(config)
                .forEach(
                    function (paramName)
                    {
                        let param = config[paramName];
                        param.set(jsonData[param.jsonEntryName]);
                    });

            console.log("Configuration chargée (version " + config.version + ").");
        });
}

/**
 * Enregistre l'état actuel de la configuration sur le serveur.
 */
function save_config()
{
    /* Construction de la chaîne de paramètres d'URL définissant la configuration actuelle. */
    let configUrlParams =
        Object.keys(config)
            .map(
                function (paramName)
                {
                    return encodeURIComponent(paramName) + "=" + encodeURIComponent(config[paramName].get());
                })
            .join('&');

    /* Écriture de la configuration actuelle sur la console. */
    Object.keys(config)
        .forEach(
            function (paramName)
            {
                console.log("[config] " + paramName + " = " + config[paramName].get());
            });

    /* Envoi de la requête de sauvegarde. */
    call_url(URL_SAVE_CONFIG + "?" + configUrlParams,
        function (responseText)
        {
            /* Redirige vers l'affichage des capteurs, si la sauvegarde a réussi. */
            redirect_to('../index.html');
        });
}

/**
 * Appelle l'URL donnée, et exécute la fonction donnée si le statut de la réponse est OK (code HTTP 200).
 * 
 * @param {string} url URL sur laquelle envoyer une requête.
 * @param {Function} onSuccess Fonction à appeler en cas de réussite.
 */
function call_url(url, onSuccess)
{
    let request = new XMLHttpRequest();

    /* Préparation de la requête. */
    request.open("GET", url, true);

    request.onreadystatechange =
        function ()
        {
            /* Si la réponse du serveur a été reçue. */
            if (request.readyState === 4)
            {
                /* Si le statut de la réponse est OK. */
                if (request.status === 200)
                {
                    onSuccess(request.responseText);
                }
                else
                {
                    alert("Une erreur est survenue:\n" + request.responseText);
                }
            }
        };

    console.log(url);

    /* Envoi de la requête. */
    request.send(null);
}

/**
 * Paramètre de configuration.
 * 
 * @param {Object} valueRef Référence vers un élément HTML, affichant la valeur de ce paramètre.
 * @param {string} jsonEntryName Nom de l'entrée JSON associée à ce paramètre.
 * @param {string} defaultValue Valeur par défaut pour ce paramètre.
 */
function Param(valueRef, jsonEntryName, defaultValue = "")
{
    this.valueRef = valueRef;
    this.jsonEntryName = jsonEntryName;
    this.get = function () { return this.valueRef.get(); };
    this.set = function (newValue) { this.valueRef.set(newValue); };

    this.set(defaultValue);
}

/**
 * Référence vers une variable JS, non liée à un élément HTML.
 */
function NoRef()
{
    this.value = "";
    this.get = function () { return this.value; };
    this.set = function (newValue) { this.value = newValue; };
}

/**
 * Référence vers un champ de saisie de formulaire.
 *
 * @param {string} formFieldId Identifiant du champ de saisie ciblé.
 */
function InputRef(formFieldId)
{
    this.formFieldId = formFieldId;
    this.get = function () { return document.getElementById(this.formFieldId).value; };
    this.set = function (newValue) { document.getElementById(this.formFieldId).value = newValue; };
}

/**
 * Référence vers une case à cocher.
 * 
 * @param {string} checkboxId Identifiant de la case à cocher ciblée.
 */
function CheckboxRef(checkboxId)
{
    this.checkboxId = checkboxId;
    this.get = function () { return document.getElementById(this.checkboxId).checked; };
    this.set = function (newValue) { document.getElementById(this.checkboxId).checked = (newValue === true || newValue === "true"); };
}

/**
 * Référence vers un bloc de texte.
 * 
 * @param {string} blockId Identifiant du bloc ciblée.
 */
function BlockContentRef(blockId)
{
    this.blockId = blockId;
    this.get = function () { return document.getElementById(blockId).innerHTML; };
    this.set = function (newValue) { document.getElementById(blockId).innerHTML = newValue; };
}
