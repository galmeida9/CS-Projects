var predictedText=["Estou a adorar descobrir Lisboa e especialmente os pastéis de Belém.",
                    "Não esperava gostar tanto de Bruxelas, surpreendeu-me imenso.",
                    "Gostava de poder ficar mais tempo em Paris para passear mais."];

var predictedTextChat = ["Olá tudo bem?", "Estou bem e tu?", "Queres ir ao cinema?", 
                        "Gostei imenso da minha viagem a Paris", "Sim", "Não", "pq?", 
                        "Estás a gostar da tua viagem?", "Estou a adorar", "Pode ser",
                        "Adeus", "Queres ir viajar comigo?"]
var Mode;
var textToPost;

/*-----------------------------------------------------------------------------------
Open Text to Speech Screen
-----------------------------------------------------------------------------------*/
function openTextToSpeech(mode) {
    document.getElementsByClassName("newPost")[0].style.display = "none";
    document.getElementsByClassName("chat")[0].style.display = "none";
    document.getElementsByClassName("microphoneChat")[0].style.display = "none";
    document.getElementsByClassName("textToSpeech")[0].style.display = "block";
    currentpage = "textToSpeech";
    Mode = mode;
}

/*-----------------------------------------------------------------------------------
Open Voice Input Confirmation Screen
-----------------------------------------------------------------------------------*/
function openShowVoiceInput() {
    document.getElementsByClassName("textToSpeech")[0].style.display = "none";
    document.getElementsByClassName("showVoiceInput")[0].style.display = "block";
    if (Mode == "post") {
        var text = Math.round(Math.random() * 2);
        document.querySelectorAll(".predictedText").forEach(function(element) {
            element.innerHTML = predictedText[text];
            textToPost = predictedText[text];
        });
    }
    else {
        var text = Math.round(Math.random() * 11);
        document.querySelectorAll(".predictedText").forEach(function(element) {
            element.innerHTML = predictedTextChat[text];
            textToPost = predictedTextChat[text];
        });
    }
    currentpage = "showVoiceInput";
    typeOfPost = 3;
}


/*-----------------------------------------------------------------------------------
Choose where to use the voice input
-----------------------------------------------------------------------------------*/
function voice() {
    if (Mode == "post") post();
    else addMessage();
}