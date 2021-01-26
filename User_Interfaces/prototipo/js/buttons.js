function menuButton() {
    if (isOpen == 0) openNav();
    else closeNav();
}

function backButton() {
    if (isOpen == 1) closeNav();
    else {
        if (currentpage == "news") {
            disableWrapper();
            disableNews();
            enableMainScreen();
            closeNav();
            currentpage = "home";
        }
        else if (currentpage == "newsStories") {
            document.getElementsByClassName("newsStory")[0].style.display = "none";
            document.getElementsByClassName("news")[0].style.display = "block";
            currentpage = "news";
            el.scrollLeft = 0;
            el.scrollTop = 0;
        }
        else if (currentpage == "newspopup") {
            document.getElementById("filterpopup").style.display = "none";
            document.getElementsByClassName("news")[0].style.display = "block";
            currentpage = "news";
        }
        else if (currentpage == "weather") {
            document.getElementsByClassName("weatherScreen")[0].style.display = "none";
            document.getElementsByClassName("news")[0].style.display = "block";
            currentpage = "news";
            el.scrollLeft = 0;
            el.scrollTop = 0;
        }
        else if (currentpage == "posts") {
            disableWrapper();
            disablePosts();
            enableMainScreen();
            currentpage = "home";
            el.scrollLeft = 0;
            el.scrollTop = 0;
        }
        else if (currentpage == "viewPost") {
            document.getElementById("viewPost").style.display = "none"
            document.getElementById("viewVideoLarge").pause();
            document.querySelectorAll(".postedVideo").forEach(function(element) {
                element.muted = true;
                element.play();
            });
            currentpage = "posts";
        }
        else if (currentpage == "newPost") {
            document.getElementsByClassName("newPost")[0].style.display = "none";
            document.getElementsByClassName("recentPosts")[0].style.display = "block";
            document.querySelectorAll(".postedVideo").forEach(function(element) {
                element.muted = true;
                element.play();
            });
            currentpage = "posts";
        }
        else if (currentpage == "settings") {
            disableSettings();
            disableWrapper();
            enableMainScreen();
            currentpage = "home";
        }
        else if (currentpage == "selectPost") {
            document.getElementsByClassName("newPost")[0].style.display = "block";
            document.getElementsByClassName("selectPost")[0].style.display = "none";
            disablePhotos();
            disableVideos();
            if (msgOrPost == "msg") currentpage = "selectAttachment";
            else currentpage = "newPost";
        }
        else if (currentpage == "confirmationPopup") {
            document.getElementById("confirmationPopup").style.display = "none";
            currentpage = "selectPost";
        }
        else if (currentpage == "locationShare") {
            document.getElementsByClassName("locationShare")[0].style.display = "none";
            document.getElementsByClassName("newPost")[0].style.display = "block";
            currentpage = "newPost";
        }
        else if (currentpage == "textToSpeech") {
            document.getElementsByClassName("textToSpeech")[0].style.display = "none";
            if (Mode == "post") {
                document.getElementsByClassName("newPost")[0].style.display = "block";
                currentpage = "newPost";
            }
            else {
                document.getElementsByClassName("chat")[0].style.display = "block";
                document.getElementsByClassName("microphoneChat")[0].style.display = "block";
                currentpage = "personChat";
            }
        }
        else if (currentpage == "showVoiceInput") {
            document.getElementsByClassName("textToSpeech")[0].style.display = "block";
            document.getElementsByClassName("showVoiceInput")[0].style.display = "none";
            currentpage = "textToSpeech";
        }
        else if (currentpage == "chat") {
            disableWrapper();
            disableChat();
            enableMainScreen();
            currentpage = "home";
        }
        else if (currentpage == "peoplepopup") {
            document.getElementById("personPopup").style.display = "none";
            currentpage = "chat";
        }
        else if (currentpage == "personChat") {
            document.getElementsByClassName("chat")[0].style.display = "none";
            document.getElementsByClassName("microphoneChat")[0].style.display = "none";
            document.getElementsByClassName("recentChat")[0].style.display = "block";
            el.scrollLeft = 0;
            el.scrollTop = 0;
            currentpage = "chat";
        }
        else if (currentpage == "notification") {
            document.getElementsByClassName("notification")[0].style.display = "none";
            currentpage = lastpage;
        }
        else if (currentpage == "sharePost") {
            document.getElementById("personPopupPost").style.display = "none";
            if (lastpage == "posts") currentpage = "posts";
            else currentpage = "newsStories";
        }
        else if (currentpage == "selectAttachment") {
            document.getElementsByClassName("newPost")[0].style.display = "none";
            document.getElementsByClassName("chat")[0].style.display = "block";
            document.getElementsByClassName("microphoneChat")[0].style.display = "block";
            document.getElementsByClassName("postScreen")[0].style.display = "none";
            el.scrollLeft = 100000;
            el.scrollTop = 100000;
            currentpage = "personChat";
        }
        else if (currentpage == "emoji") {
            document.getElementsByClassName("microphoneChat")[0].style.display = "block";
            document.getElementsByClassName("chat")[0].style.display = "block";
            document.getElementsByClassName("selectEmoji")[0].style.display = "none";
            el.scrollLeft = 100000;
            el.scrollTop = 100000;
            currentpage = "personChat";
        }
        else if (currentpage == "help") {
            document.getElementsByClassName("help")[0].style.display = "none";
            document.getElementsByClassName("settings")[0].style.display = "block";
            currentpage = "settings";
        }
        else if (currentpage == "confirmationPopupShare") {
            cancelShare();
        }
    }
}