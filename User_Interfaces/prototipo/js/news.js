/*-------------------------------------------------------------------------------------
Global variables
--------------------------------------------------------------------------------------*/

var newsStories = {
    "mocambique": {
        title: "501 mortos em Moçambique",
        text: "O número de feridos manteve-se em 1.523, mas o total de pessoas afetadas subiu para 843.723. O grupo de pessoas afetadas inclui todas aquelas que perderam as casas, precisam de alimentos ou de algum tipo de assistência, explicaram as autoridades. O total de desalojados em centros de acolhimento mantém-se em 140.784, assim como o número de famílias beneficiárias de assistência humanitária: 29.098. Os outros dados publicados pelo Instituto Nacional de Gestão de Calamidades (INGC) que hoje sofreram alterações foram o total de salas de aula afetadas, que subiu para 3.318, com 150.854 alunos prejudicados, e o número de casas totalmente destruídas que ascende 56.095 (com outras 28.129 parcialmente danificadas), sendo que a maioria são habitações de construção precária. Centenas de milhares de pessoas necessitam de ajuda nos três países afetados: Moçambique, Zimbabué e Maláui. Até hoje, estão contabilizados 746 mortos nos três países, segundo as autoridades locais e a ONU.",
    }, 
    "futebol": {
        title:"Felizmente a Federação Portuguesa de Futebol teve o bom-senso que a RTP não teve",
        text: "António Costa diz que o Governo está a analisar os esclarecimentos da RTP sobre o acordo que fez com a Federação Portuguesa de Futebol e que, entretanto, caiu. Depois dessa análise, serão tomadas medidas.",
    },
    "lanche": {
        title: "Lanches das crianças do pré-escolar vão ter fruta e legumes",
        text: "A distribuição de fruta e legumes nos lanches dos alunos do 1.º ciclo vai ser alargada ao pré-escolar, anunciou a responsável pelo programa da Direção-Geral da Saúde (DGS), que reconhece que a iniciativa tem ficado aquém do esperado.",
    },
    "brexit": {
        title: "Brexit: o que os 400 mil portugueses têm de fazer para permanecer no Reino Unido?",
        text: "Depois de o Parlamento britânico ter rejeitado pela terceira vez o acordo de saída da UE, o Brexit fica agora agendado para o dia 12 de abril. Mas como fica a situação dos 400 mil portugueses que vivem no Reino Unido da Grã-Bretanha (que abrange Inglaterra, Escócia e País de Gales) e Irlanda do Norte? Os portugueses que não tenham a cidadania britânica têm de fazer registo num status específico, baseado nos anos que já passaram a viver naquele país, ou seja: Recorrem ao settled status – estatuto de residência permanente –, se moram há cinco anos ou mais no Reino Unido; Registam-se ao pre-settled status – estatuto de residência temporária –, para quem mora há menos de cinco anos. Clique aqui para fazer o registo.",
    },
    "bebe": {
        title: "Como se mantém vivo um bebé na barriga de uma mulher em morte cerebral",
        text: "Ao fim de menos de 32 semanas de gestação, no total, Salvador teve de ser retirado do ventre da mãe mais cedo do que se esperava. Os médicos já não estavam a conseguir sustentar, através das máquina, a respiração de Catarina Sequeira, diagnosticada com morte cerebral em dezembro. O oxigénio não estava a chegar à criança, que corria já mais riscos dentro da barriga da mãe do que se viesse ao mundo condenado a passar os primeiros tempos de vida na incubadora. A última palavra foi, como é sempre, dos obstetras. Injetaram uma hormona esteroide chamada corticosteroide que amadureceu à pressa os pulmões, intestinos e sistema nervoso central do feto. Prepararam Catarina para a cesariana. E, durante a madrugada, às 04h23, nasceu Salvador, o segundo bebé milagre português, gerado dentro de uma mãe clinicamente morta. Tinha 1.700 gramas, 40 centímetros. E outra batalha para enfrentar. Outra, porque a primeira de todas começou quando Salvador ainda só pesava 240 gramas e tinha o tamanho de uma maçã. A mãe, uma mulher de 26 anos, teve um ataque de asma que lhe desligou a atividade cerebral para sempre. Não há volta a dar. O coração bate, os intestinos e os rins ainda funcionam, mas nem o cérebro nem o tronco cerebral respondem a qualquer estímulo. Catarina está morta, clinicamente, desde 20 de dezembro. Mas continuou ligada às máquinas até esta quinta-feira, para dar vida ao filho. Durante estas 13 semanas, foi numa “incubadora humana”, adjetivou a avó da criança.",
    }
}

document.getElementById("allCheck").style.display = "block";
var newsCheck = ["allCheck", "localCheck", "politicsCheck", "scienceCheck", "sportsCheck"];
var newsCategories = ["local", "politics", "sport", "science"];
var weatherDays = ["weatherDay2", "weatherDay3", "weatherDay4", "weatherDay5", "weatherDay6", "weatherDay7"];
var days = ["day2", "day3", "day4", "day5", "day6", "day7"];
var weatherIcons = ["resources/sunny.png", "resources/cloud.png", "resources/rain.png"];
var weatherImg = ["weatherImg2", "weatherImg3", "weatherImg4", "weatherImg5", "weatherImg6", "weatherImg7"];
var numberOfNews = 5;
var numberOfCategories = 5;
var currentNews;
weather();


/*-------------------------------------------------------------------------------------
Enables/Disables News 
--------------------------------------------------------------------------------------*/
function disableNews() {
    document.getElementsByClassName("newsscreen")[0].style.display = "none";
    document.getElementById("filterpopup").style.display = "none";
    document.getElementsByClassName("weatherScreen")[0].style.display = "none";
}

function enableNews() {
    document.getElementsByClassName("newsscreen")[0].style.display = "block";
    document.getElementsByClassName("newsStory")[0].style.display = "none";
    document.getElementsByClassName("news")[0].style.display = "block";
    document.getElementsByClassName("weatherScreen")[0].style.display = "none";
    document.getElementById("filterpopup").style.display = "none";
}


/*-------------------------------------------------------------------------------------
Displays weather function
--------------------------------------------------------------------------------------*/

function weather() {
    var i;
    var date;
    var random;
    var weather = Math.round(Math.random() * 35);
    weather += "°C";
    document.querySelectorAll(".weatherToday").forEach(function(element) {
        element.innerHTML = weather;
    });

    for(i = 0; i < 6; i++) {
        weather = Math.round(Math.random() * 35);
        weather += "°C";
        document.getElementById(weatherDays[i]).innerHTML = weather;

        date = new Date(new Date().getTime() + (i+1) * 24 * 60 * 60 * 1000);
        day = date.getDate();
        month = date.getMonth() + 1;
        if (day < 10) day = "0" + day;
        if (month < 10) month = "0" + month;

        document.getElementById(days[i]).innerHTML = day + "/" + month;

        random = Math.round(Math.random() * 2);
        document.getElementById(weatherImg[i]).src = weatherIcons[random];
    }
}


/*-------------------------------------------------------------------------------------
Opens weather page
--------------------------------------------------------------------------------------*/
function openWeather() {
    currentpage = "weather";
    document.getElementsByClassName("news")[0].style.display = "none";
    document.getElementsByClassName("weatherScreen")[0].style.display = "block";
}


/*-------------------------------------------------------------------------------------
Opens selected news story
--------------------------------------------------------------------------------------*/
function showNews(News) {
    if (mouseMovement < 3 && mouseMovement > -3) {
        el.scrollLeft = 0;
        el.scrollTop = 0;
        currentpage = "newsStories";
        document.getElementsByClassName("news")[0].style.display = "none";
        document.getElementsByClassName("newsStory")[0].style.display = "block";
        document.querySelectorAll(".newsPart").forEach(function(element) {
            element.innerHTML = newsStories[News][element.id];
        });
        currentNews = News;
    }
}


/*-------------------------------------------------------------------------------------
Post News
--------------------------------------------------------------------------------------*/
function postNews() {
    if (share == 1) {
        typeOfPost = 3;
        textToPost = "News Story: ".bold() + newsStories[currentNews]["title"];
        msgOrPost = "posts";
        post();
    }
    document.getElementById("confirmationPopupShare").style.display = "none";
}

function sendNews() {
    textToPost = "News Story: ".bold() + newsStories[currentNews]["title"];
    lastpage = "newsStories";
    selectPerson(3, textToPost);
}


/*-------------------------------------------------------------------------------------
Filter news category
--------------------------------------------------------------------------------------*/
document.getElementById("filter").onclick = function() {
    document.getElementById("filterpopup").style.display = "block";
    currentpage = "newspopup";
}

document.getElementsByClassName("close")[0].onclick = function() {
    document.getElementById("filterpopup").style.display = "none";
    currentpage = "news";
}

/*-------------------------------------------------------------------------------------
Filter news category
--------------------------------------------------------------------------------------*/
function showLocal() {
    var i;
    for (i = 0; i < newsCheck.length; i++) {
        if (newsCheck[i] == "localCheck") document.getElementById(newsCheck[i]).style.display = "block";
        else document.getElementById(newsCheck[i]).style.display = "none";
    }
    for (i = 0; i < newsCategories.length; i++) {
        if (newsCategories[i] == "local") document.getElementById(newsCategories[i]).style.display = "block";
        else document.getElementById(newsCategories[i]).style.display = "none";
    }
    document.getElementById("filterpopup").style.display = "none";
    currentpage = "news";
}

function showPolitics() {
    var i;
    for (i = 0; i < newsCheck.length; i++) {
        if (newsCheck[i] == "politicsCheck") document.getElementById(newsCheck[i]).style.display = "block";
        else document.getElementById(newsCheck[i]).style.display = "none";
    }
    for (i = 0; i < newsCategories.length; i++) {
        if (newsCategories[i] == "politics") document.getElementById(newsCategories[i]).style.display = "block";
        else document.getElementById(newsCategories[i]).style.display = "none";
    }
    document.getElementById("filterpopup").style.display = "none";
    currentpage = "news";
}

function showScience() {
    var i;
    for (i = 0; i < newsCheck.length; i++) {
        if (newsCheck[i] == "scienceCheck") document.getElementById(newsCheck[i]).style.display = "block";
        else document.getElementById(newsCheck[i]).style.display = "none";
    }
    for (i = 0; i < newsCategories.length; i++) {
        if (newsCategories[i] == "science") document.getElementById(newsCategories[i]).style.display = "block";
        else document.getElementById(newsCategories[i]).style.display = "none";
    }
    document.getElementById("filterpopup").style.display = "none";
    currentpage = "news";
}

function showSports() {
    var i;
    for (i = 0; i < newsCheck.length; i++) {
        if (newsCheck[i] == "sportsCheck") document.getElementById(newsCheck[i]).style.display = "block";
        else document.getElementById(newsCheck[i]).style.display = "none";
    }
    for (i = 0; i < newsCategories.length; i++) {
        if (newsCategories[i] == "sports") document.getElementById(newsCategories[i]).style.display = "block";
        else document.getElementById(newsCategories[i]).style.display = "none";
    }
    document.getElementById("filterpopup").style.display = "none";
    currentpage = "news";
}

function showAll() {
    var i;
    for (i = 0; i < newsCheck.length; i++) {
        if (newsCheck[i] == "allCheck") document.getElementById(newsCheck[i]).style.display = "block";
        else document.getElementById(newsCheck[i]).style.display = "none";
    }
    for (i = 0; i < newsCategories.length; i++) {
        document.getElementById(newsCategories[i]).style.display = "block";
    }
    document.getElementById("filterpopup").style.display = "none";
    currentpage = "news";
}