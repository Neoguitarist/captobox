/** URL � appeler pour r�cup�rer la configuration enregistr�e sur le serveur. */
const URL_LOAD_CONFIG = "/lecture_config";
/** URL � appeler pour sauvegarder la configuration sur le serveur. */
const URL_SAVE_CONFIG = "write";

/** Configuration manipul�e. */
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
 * Charge la configuration enregistr�e sur le serveur.
 */
function load_config()
{
    /* Envoi une requ�te pour r�cup�rer la configuration enregistr�e sur le serveur. */
    call_url(URL_LOAD_CONFIG,
        function (responseText)
        {
            let jsonData = JSON.parse(responseText);

            /* Chargement de la configuration re�ue. */
            Object.keys(config)
                .forEach(
                    function (paramName)
                    {
                        let param = config[paramName];
                        param.set(jsonData[param.jsonEntryName]);
                    });

            console.log("Configuration charg�e (version " + config.version + ").");
        });
}

/**
 * Enregistre l'�tat actuel de la configuration sur le serveur.
 */
function save_config()
{
    /* Construction de la cha�ne de param�tres d'URL d�finissant la configuration actuelle. */
    let configUrlParams =
        Object.keys(config)
            .map(
                function (paramName)
                {
                    return encodeURIComponent(paramName) + "=" + encodeURIComponent(config[paramName].get());
                })
            .join('&');

    /* �criture de la configuration actuelle sur la console. */
    Object.keys(config)
        .forEach(
            function (paramName)
            {
                console.log("[config] " + paramName + " = " + config[paramName].get());
            });

    /* Envoi de la requ�te de sauvegarde. */
    call_url(URL_SAVE_CONFIG + "?" + configUrlParams,
        function (responseText)
        {
            /* Redirige vers l'affichage des capteurs, si la sauvegarde a r�ussi. */
            redirect_to('../index.html');
        });
}

/**
 * Appelle l'URL donn�e, et ex�cute la fonction donn�e si le statut de la r�ponse est OK (code HTTP 200).
 * 
 * @param {string} url URL sur laquelle envoyer une requ�te.
 * @param {Function} onSuccess Fonction � appeler en cas de r�ussite.
 */
function call_url(url, onSuccess)
{
    let request = new XMLHttpRequest();

    /* Pr�paration de la requ�te. */
    request.open("GET", url, true);

    request.onreadystatechange =
        function ()
        {
            /* Si la r�ponse du serveur a �t� re�ue. */
            if (request.readyState === 4)
            {
                /* Si le statut de la r�ponse est OK. */
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

    /* Envoi de la requ�te. */
    request.send(null);
}

/**
 * Param�tre de configuration.
 * 
 * @param {Object} valueRef R�f�rence vers un �l�ment HTML, affichant la valeur de ce param�tre.
 * @param {string} jsonEntryName Nom de l'entr�e JSON associ�e � ce param�tre.
 * @param {string} defaultValue Valeur par d�faut pour ce param�tre.
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
 * R�f�rence vers une variable JS, non li�e � un �l�ment HTML.
 */
function NoRef()
{
    this.value = "";
    this.get = function () { return this.value; };
    this.set = function (newValue) { this.value = newValue; };
}

/**
 * R�f�rence vers un champ de saisie de formulaire.
 *
 * @param {string} formFieldId Identifiant du champ de saisie cibl�.
 */
function InputRef(formFieldId)
{
    this.formFieldId = formFieldId;
    this.get = function () { return document.getElementById(this.formFieldId).value; };
    this.set = function (newValue) { document.getElementById(this.formFieldId).value = newValue; };
}

/**
 * R�f�rence vers une case � cocher.
 * 
 * @param {string} checkboxId Identifiant de la case � cocher cibl�e.
 */
function CheckboxRef(checkboxId)
{
    this.checkboxId = checkboxId;
    this.get = function () { return document.getElementById(this.checkboxId).checked; };
    this.set = function (newValue) { document.getElementById(this.checkboxId).checked = (newValue === true || newValue === "true"); };
}

/**
 * R�f�rence vers un bloc de texte.
 * 
 * @param {string} blockId Identifiant du bloc cibl�e.
 */
function BlockContentRef(blockId)
{
    this.blockId = blockId;
    this.get = function () { return document.getElementById(blockId).innerHTML; };
    this.set = function (newValue) { document.getElementById(blockId).innerHTML = newValue; };
}
