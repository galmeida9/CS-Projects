var imgToPost;
var videoToPost;
var typeOfPost = 0; /* {0:video, 1:image; 2:location; 3:voice input} */
var contentToSend;


/*-----------------------------------------------------------------------------------
Disable/Enable Posts
-----------------------------------------------------------------------------------*/
function disablePosts() {
    document.getElementsByClassName("postScreen")[0].style.display = "none";
    document.getElementsByClassName("newPost")[0].style.display = "none";
    document.getElementsByClassName("recentPosts")[0].style.display = "none";
    document.getElementsByClassName("locationShare")[0].style.display = "none";
    document.getElementsByClassName("textToSpeech")[0].style.display = "none";
    document.getElementsByClassName("showVoiceInput")[0].style.display = "none";
    document.getElementById("viewPost").style.display = "none";
    document.getElementById("viewVideoLarge").pause();
    document.getElementById("confirmationPopup").style.display = "none";
    document.querySelectorAll(".postedVideo").forEach(function(element) {
        element.pause();
    });
    disablePhotos();
    disableVideos();

}

function enablePosts() {
    document.getElementsByClassName("postScreen")[0].style.display = "block";
    document.getElementsByClassName("newPost")[0].style.display = "none";
    document.getElementsByClassName("textToSpeech")[0].style.display = "none";
    document.getElementsByClassName("showVoiceInput")[0].style.display = "none";
    document.getElementsByClassName("locationShare")[0].style.display = "none";
    document.getElementsByClassName("recentPosts")[0].style.display = "block";
    document.getElementById("viewPost").style.display = "none";
    document.getElementById("viewVideoLarge").pause();
    document.getElementById("confirmationPopup").style.display = "none";
    document.querySelectorAll(".postedVideo").forEach(function(element) {
        element.play();
        element.muted = true;
    });
    disablePhotos();
    disableVideos();
    lastpage = "posts";
}


/*-----------------------------------------------------------------------------------
View images fullscreen
-----------------------------------------------------------------------------------*/
function viewImg(img){
    if (mouseMovement < 3 && mouseMovement > -3) {
        document.getElementById("viewPost").style.display = "block";
        document.getElementById("viewImgLarge").style.display = "block";
        document.getElementById("viewVideoLarge").style.display = "none";
        document.getElementById("viewImgLarge").src = img;
        document.querySelectorAll(".postedVideo").forEach(function(element) {
            element.pause();
        });
        currentpage = "viewPost";
    }
}


/*-----------------------------------------------------------------------------------
View videos fullscreen
-----------------------------------------------------------------------------------*/
function viewVideo(video){
    if (mouseMovement < 3 && mouseMovement > -3) {
        document.getElementById("viewPost").style.display = "block";
        document.getElementById("viewImgLarge").style.display = "none";
        document.getElementById("viewVideoLarge").style.display = "block";
        document.getElementById("viewVideoLarge").src = video;
        document.querySelectorAll(".postedVideo").forEach(function(element) {
            element.pause();
        });
        document.getElementById("viewVideoLarge").play();
        currentpage = "viewPost";
    }
}


/*-----------------------------------------------------------------------------------
Enable new post screen
-----------------------------------------------------------------------------------*/
function newPost(){
    document.getElementsByClassName("recentPosts")[0].style.display = "none";
    document.getElementsByClassName("newPost")[0].style.display = "block";
    document.getElementById("selectMicrophone").style.display = "block";
    document.getElementById("voiceToTxt").style.display = "block";
    document.querySelectorAll(".postedVideo").forEach(function(element) {
        element.pause();
    });
    currentpage = "newPost"
}


/*-----------------------------------------------------------------------------------
Select video or image post
-----------------------------------------------------------------------------------*/
function selectPost(mode){
    document.getElementsByClassName("postScreen")[0].style.display = "block";
    document.getElementsByClassName("newPost")[0].style.display = "none";
    document.getElementsByClassName("selectPost")[0].style.display = "block";
    el.scrollLeft = 0;
    el.scrollTop = 0;
    if (mode == "photo") enablePhotos();
    else enableVideos();
    currentpage = "selectPost";
}


/*-----------------------------------------------------------------------------------
Confirm post
-----------------------------------------------------------------------------------*/
function postConfirmation(source, mode){
    if (mouseMovement < 3 && mouseMovement > -3) {
        document.getElementById("confirmationPopup").style.display = "block";
        currentpage = "confirmationPopup";
        if (mode == "photo") { imgToPost = source; typeOfPost = 1; contentToSend = source;}
        else { videoToPost = source; typeOfPost = 0; contentToSend = source;}
    }
}


/*-----------------------------------------------------------------------------------
Post img/Video
-----------------------------------------------------------------------------------*/
function post(){
    if (msgOrPost == "posts") {
        var lastPost = document.getElementsByClassName("postItem")[0];
        var newPost = document.createElement("div");
        newPost.classList.add("postItem");
        
        if (typeOfPost == 1) 
            newPost.innerHTML = "<p class='postItemText darkmodeText'><img class='user userdarkmode' src='resources/user.png'> <b>You</b> posted</p><img class='postedImg' src=" + imgToPost + " onclick=" + "viewImg('" + imgToPost + "')" + "><img class='sharePost' src='resources/share.png' style='width: 0.5cm; cursor: pointer' onclick=\42selectPerson(1, \47" + imgToPost + "\47)\42><br>";
        else if (typeOfPost == 0)
            newPost.innerHTML = "<p class='postItemText darkmodeText'><img class='user userdarkmode' src='resources/user.png'> <b>You</b> posted</p><Video class='postedVideo' src=" + videoToPost + " onclick=" + "viewVideo('" + videoToPost + "')" + "></Video><img class='sharePost' src='resources/share.png' style='width: 0.5cm; cursor: pointer' onclick=\42selectPerson(0, \47" + videoToPost + "\47)\42><br>";
        else if (typeOfPost == 2)
            newPost.innerHTML = "<p class='postItemText darkmodeText'><img class='user userdarkmode' src='resources/user.png'> <b>You</b> posted</p><img class='postedImg' src='resources/tecnico_location.png' onclick=" + "viewImg('resources/tecnico_location.png')" + "><img class='sharePost' src='resources/share.png' style='width: 0.5cm; cursor: pointer' onclick=\42selectPerson(1, 'resources/tecnico_location.png')\42><br>";
        else
            newPost.innerHTML = "<p class='postItemText darkmodeText'><img class='user userdarkmode' src='resources/user.png'> <b>You</b> posted</p><p class='postItemText darkmodeText'>" + textToPost + "<br><img class='sharePost' src='resources/share.png' style='width: 0.5cm; cursor: pointer' onclick=\42selectPerson(3, \47" + textToPost + "\47)\42>";

        lastPost.insertAdjacentElement("beforebegin", newPost);
        if (nightmodeOn == 1) {
            document.querySelectorAll(".darkmodeText").forEach(function(element) {
                element.style.color = 'white';
            });
            document.querySelectorAll(".sharePost").forEach(function(element) {
                element.src="resources/share_white.png";
            });
            document.getElementsByClassName("user")[0].src="resources/user_white.png";
        }
        disablePhotos();
        disableNews();
        enablePosts();
        el.scrollLeft = 0;
        el.scrollTop = 0;
        currentpage = "posts";
    }
    else if (msgOrPost == "msg"){
        share = 1;
        sharePost(currentPerson);
    }
    else{
        if (typeOfPost == 0) {selectPerson(0, videoToPost); sharePost(currentPerson);}
        else if (typeOfPost == 1) {selectPerson(1, imgToPost); sharePost(currentPerson);}
        else {selectPerson(1, 'resources/tecnico_location.png'); sharePost(currentPerson);}

    }
}


/*-----------------------------------------------------------------------------------
Open Share Location Screen
-----------------------------------------------------------------------------------*/
function openShareLocation() {
    document.getElementsByClassName("newPost")[0].style.display = "none";
    document.getElementsByClassName("locationShare")[0].style.display = "block";
    currentpage = "locationShare";
    typeOfPost = 2;
    contentToSend = "resources/tecnico_location.png";
}


/*-----------------------------------------------------------------------------------
Disable/Enable photos
-----------------------------------------------------------------------------------*/
function enablePhotos() {
    document.querySelectorAll(".selectIMG").forEach(function(element) {
        element.style.display = "block";
    });
}

function disablePhotos() {
    document.querySelectorAll(".selectIMG").forEach(function(element) {
        element.style.display = "none";
    });
}


/*-----------------------------------------------------------------------------------
Disable/Enable videos
-----------------------------------------------------------------------------------*/
function enableVideos() {
    document.querySelectorAll(".selectVideo").forEach(function(element) {
        element.style.display = "block";
        element.play();
        element.muted = true;
    });
}

function disableVideos() {
    document.querySelectorAll(".selectVideo").forEach(function(element) {
        element.style.display = "none";
        element.pause();
        element.muted = false;
    });
}

/*-----------------------------------------------------------------------------------
Select Person to share
-----------------------------------------------------------------------------------*/
function selectPerson(type, src) {
    if ((mouseMovement < 3 && mouseMovement > -3)) {
        document.getElementById("personPopupPost").style.display="block";
        typeOfPost = type;
        contentToSend = src;
        currentpage = "sharePost";
        lastpage2 = currentpage;
    }
}

document.getElementsByClassName("close")[1].onclick = function() {
    document.getElementById("personPopupPost").style.display = "none";
    if (lastpage == "posts") currentpage = "posts";
    else currentpage = "newsStories";
}

/*-----------------------------------------------------------------------------------
Share Post
-----------------------------------------------------------------------------------*/
function sharePost(person) {
    if (share == 0) {
        document.getElementById("confirmationPopupShare").style.display = "none";
    }
    else if (share == 1) {
        document.getElementById("confirmationPopupShare").style.display = "none";
        var random =  Math.round(Math.random() * 13) + 2;
        document.getElementById("personPopupPost").style.display="none";
        if (typeOfPost == 0) {
            allMessages[person]["Messages"].push("<div class='messageSent'><span><img class='userdarkmode' src='resources/user.png'><button class='btn'><Video class='chatIMG chatVideo' src=" + contentToSend + " style='width:auto; margin-top:0pt;'></button></span></div>");
            allMessages[person]["Number"] += 1;
            lastMessage[person] = "Video";
            updateLastMessage(person, "Video");
            addPerson(person);
        }
        else if (typeOfPost == 1 || typeOfPost == 2) {
            allMessages[person]["Messages"].push("<div class='messageSent'><span><img class='userdarkmode' src='resources/user.png'><button class='btn'><img class='chatIMG' src=" + contentToSend + " style='width:auto; margin-top:0pt;'></button></span></div>");
            allMessages[person]["Number"] += 1;
            lastMessage[person] = "Image";
            updateLastMessage(person, "Image");
            addPerson(person);
        }
        else if (typeOfPost == 3) {
            allMessages[person]["Messages"].push("<div class='messageSent'><span><img class='userdarkmode' src='resources/user.png'><button class='btn'><p>" + contentToSend + "</p></button></span></div>");
            allMessages[person]["Number"] += 1;
            lastMessage[person] = contentToSend;
            updateLastMessage(person, contentToSend);
            addPerson(person);
        }
        setTimeout(function(){reply(person);}, random*1000);
    }
}


/*-----------------------------------------------------------------------------------
Share Post To person Popup
-----------------------------------------------------------------------------------*/
function showPopupShare(person, mode) {
    document.getElementById("confirmationPopupShare").style.display = "block";
    currentPerson = person;
    sharePersonMode = mode;
    currentpage = "confirmationPopupShare";
    if (mode == 2) lastpage2 = "newsStories";
}

function cancelShare() {
    share = 0; 
    currentpage = lastpage2;
    if (sharePersonMode == 0) sharePost(currentPerson);
    else postNews();
}
function confirmShare() {
    share = 1; 
    currentpage = lastpage2;
    if (sharePersonMode == 0) sharePost(currentPerson);
    else postNews();
}
