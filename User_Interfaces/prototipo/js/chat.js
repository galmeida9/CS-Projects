var allMessages = {"Gabriel": {
                        Messages: ["<div class='messageReceived'><span><img src='resources/gabriel_user.png'><button class='btn'><p>Olá</p></button></span></div>",
                                    "<div class='messageSent'><span><img class='userdarkmode' src='resources/user.png'><button class='btn'><p>Mekie</p></button></span></div>",
                                    "<div class='messageReceived'><span><img src='resources/gabriel_user.png'><button class='btn'><p>tudo bem?</p></button></span></div>",
                                    "<div class='messageSent'><span><img class='userdarkmode' src='resources/user.png'><button class='btn'><p>yh e ctg?</p></button></span></div>",
                                    "<div class='messageReceived'><span><img src='resources/gabriel_user.png'><button class='btn'><p>Também</p></button></span></div>",
                                    "<div class='messageSent'><span><img class='userdarkmode' src='resources/user.png'><button class='btn'><p>Já viste os Avengers?</p></button></span></div>",
                                    "<div class='messageReceived'><span><img src='resources/gabriel_user.png'><button class='btn'><p>Foi brutal!!!</p></button></span></div>"],
                        src: "resources/gabriel_user.png",
                        Number: 7,
                        },
                "João" : {
                        Messages: ["<div class='messageReceived'><span><img src='resources/joao_user.png'><button class='btn'><p>Boas mano!</p></button></span></div>",
                                    "<div class='messageSent'><span><img class='userdarkmode' src='resources/user.png'><button class='btn'><p>Então mano!</p></button></span></div>",
                                    "<div class='messageReceived'><span><img src='resources/joao_user.png'><button class='btn'><p>Queres ir ao cinema?</p></button></span></div>"],
                        src: "resources/joao_user.png",
                        Number: 3,
                        },
                "Daniel": {
                        Messages: ["<div class='messageReceived'><span><img src='resources/daniel_user.png'><button class='btn'><p>Bora almoçar fora?</p></button></span></div>"],
                        src: "resources/daniel_user.png",
                        Number: 1,
                        },
                "Diogo": {
                        Messages: [],
                        src: "resources/user.png",
                        Number: 0,
                        },
                "Bernardo": {
                        Messages: [],
                        src: "resources/bernardo_user.png",
                        Number: 0,
                        }
                };

var lastMessage = {"Gabriel": "Foi brutal!!!",
                    "João" : "Queres ir ao cinema?",
                    "Daniel": "Bora almoçar fora?",
                    "Diogo": "",
                    "Bernardo": ""
                    };

var displayedMessages = 0;
var personInMemory = ["Gabriel", "João", "Daniel"];
var currentPerson;
var notificationPerson;
var share = -1;
var sharePersonMode = -1;

/*-----------------------------------------------------------------------------------
Disable/Enable Chat
-----------------------------------------------------------------------------------*/
function disableChat() {
    document.getElementsByClassName("chatScreen")[0].style.display = "none";
    document.getElementsByClassName("recentChat")[0].style.display = "none";
    document.getElementsByClassName("textToSpeech")[0].style.display = "none";
    document.getElementsByClassName("showVoiceInput")[0].style.display = "none";
    document.getElementsByClassName("selectEmoji")[0].style.display = "none";
    document.getElementsByClassName("chat")[0].style.display = "none";
    document.getElementsByClassName("microphoneChat")[0].style.display = "none";
}

function enableChat() {
    document.getElementsByClassName("chatScreen")[0].style.display = "block";
    document.getElementsByClassName("recentChat")[0].style.display = "block";
    document.getElementsByClassName("selectEmoji")[0].style.display = "none";
}

/*-----------------------------------------------------------------------------------
Chat Popup for choosing people
-----------------------------------------------------------------------------------*/
document.getElementById("addChat").onclick = function() {
    document.getElementById("personPopup").style.display = "block";
    currentpage = "peoplepopup";
}

document.getElementsByClassName("close")[2].onclick = function() {
    document.getElementById("personPopup").style.display = "none";
    currentpage = "chat";
}


/*-----------------------------------------------------------------------------------
Open Chat
-----------------------------------------------------------------------------------*/
function chat(person) {
    msgOrPost = "msg";
    if ((mouseMovement < 3 && mouseMovement > -3) || currentpage != "chat") {
        currentPerson = person;
        el.scrollLeft = 0;
        el.scrollTop = 0;
        openChat();
        document.getElementsByClassName("recentChat")[0].style.display = "none";
        document.getElementsByClassName("chat")[0].style.display = "block";
        document.getElementsByClassName("microphoneChat")[0].style.display = "block";
        
        var messages = document.getElementsByClassName("chat")[0];
        var i=0, k = 1;

        while (k <= displayedMessages) {
            messages.removeChild(messages.childNodes[0]);
            k++;
        }
        if(k!=1) messages.removeChild(messages.childNodes[0]);

        displayedMessages = 0;
        while (i<allMessages[person]["Number"]){
                var newMessage = document.createElement("div");
                newMessage.innerHTML = allMessages[person]["Messages"][i];
                messages.appendChild(newMessage);
                i++;
                displayedMessages++;
        }
        var newMessage = document.createElement("div");
        newMessage.innerHTML = "<br><br>";
        messages.appendChild(newMessage);

        if (nightmodeOn == 1) {
            document.querySelectorAll(".userdarkmode").forEach(function(element) {
                element.src="resources/user_white.png";
            });
        }

        document.querySelectorAll(".chatVideo").forEach(function(element) {
            element.play();
            element.muted = true;
        });

        el.scrollTop = 100000;
        currentpage="personChat";
    }
}


/*-----------------------------------------------------------------------------------
Add Person Chat
-----------------------------------------------------------------------------------*/
function addPerson(person) {
    
    document.getElementById("personPopup").style.display = "none";
    if (personInMemory.includes(person)) chat(person);
    else {
        personInMemory += person;

        var chatScreen = document.getElementsByClassName("recentChat")[0];
        var newChat = document.createElement("div");
        newChat.innerHTML = "<button class='btn chatPerson' style='margin-top: 4pt; height: 27pt; cursor:pointer; text-align: left;  overflow: hidden;' onclick=\42chat(\47" + person + "\47)\42><h2 style='font-weight: bold'><img class='user' src=" + allMessages[person]["src"] + " style='margin-top: -2pt'> " + person + "</h2><p class='lastMessage " + person + "'>" + lastMessage[person] + "</p><br><br><br></button>"
        chatScreen.appendChild(newChat);
        chat(person);
    }
}


/*-----------------------------------------------------------------------------------
Send Message
-----------------------------------------------------------------------------------*/
function addMessage() {
    var random =  Math.round(Math.random() * 13) + 2;
    var person = currentPerson;

    allMessages[currentPerson]["Messages"].push("<div class='messageSent'><span><img class='userdarkmode' src='resources/user.png'><button class='btn'><p>" + textToPost + "</p></button></span></div>");
    allMessages[currentPerson]["Number"] += 1;
    document.getElementsByClassName("showVoiceInput")[0].style.display = "none";
    lastMessage[currentPerson] = textToPost;
    updateLastMessage(person, textToPost);
    chat(currentPerson);
    setTimeout(function(){reply(person);}, random*1000);
    console.log(random);
}

/*-----------------------------------------------------------------------------------
Receive Message
-----------------------------------------------------------------------------------*/
function reply(person) {
    var random =  Math.round(Math.random() * 11);
    var querry = "." + person;

    if (allMessages[person]["src"] == "resources/user.png") allMessages[person]["Messages"].push("<div class='messageReceived'><span><img class='userdarkmode' src=" + allMessages[person]["src"] + "><button class='btn'><p>" + predictedTextChat[random] + "</p></button></span></div>");
    else allMessages[person]["Messages"].push("<div class='messageReceived'><span><img src=" + allMessages[person]["src"] + "><button class='btn'><p>" + predictedTextChat[random] + "</p></button></span></div>");
    allMessages[person]["Number"] += 1;
    document.querySelectorAll(querry).forEach(function(element) {
        element.innerHTML = predictedTextChat[random];
    });
    lastMessage[person] = predictedTextChat[random];
    showNotification(person);
    console.log(person + " sent a message");
}

/*-----------------------------------------------------------------------------------
Get Notification
-----------------------------------------------------------------------------------*/
function showNotification(person){
    document.getElementsByClassName("notification")[0].style.display = "block";
    document.getElementById("notificationIMG").src = allMessages[person]["src"];
    document.getElementById("notificationTXT").innerHTML = lastMessage[person];
    notificationPerson = person;
    lastpage = currentpage;
    currentpage = "notification";
}

/*-----------------------------------------------------------------------------------
Open Notification
-----------------------------------------------------------------------------------*/
function openNotification(){
    document.getElementsByClassName("notification")[0].style.display = "none";
    chat(notificationPerson);
}

/*-----------------------------------------------------------------------------------
Chat Attachment
-----------------------------------------------------------------------------------*/
function chatAttachment() {
    document.getElementsByClassName("chat")[0].style.display = "none";
    document.getElementsByClassName("microphoneChat")[0].style.display = "none";
    document.getElementsByClassName("newPost")[0].style.display = "block";
    document.getElementById("selectMicrophone").style.display = "none";
    document.getElementById("voiceToTxt").style.display = "none";
    currentpage = "selectAttachment";
}


/*-----------------------------------------------------------------------------------
Update last message displayed
-----------------------------------------------------------------------------------*/
function updateLastMessage(person, text) {
    var querry = "." + person;
    document.querySelectorAll(querry).forEach(function(element) {
        element.innerHTML = text;
    });
}

/*-----------------------------------------------------------------------------------
Open Emojis Screen
-----------------------------------------------------------------------------------*/
function openEmojis() {
    document.getElementsByClassName("chat")[0].style.display = "none";
    document.getElementsByClassName("microphoneChat")[0].style.display = "none";
    document.getElementsByClassName("selectEmoji")[0].style.display = "block";
    el.scrollLeft = 0;
    el.scrollTop = 0;
    currentpage = "emoji";
}


/*-----------------------------------------------------------------------------------
Send Emoji
-----------------------------------------------------------------------------------*/
function shareEmoji(emoji) {
    if (mouseMovement < 3 && mouseMovement > -3) {
        typeOfPost = 1;
        contentToSend = emoji;
        share = 1;
        sharePost(currentPerson);
    }
}