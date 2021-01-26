var isOpen = 0;


/*-----------------------------------------------------------------------------------
Open/Close menu
-----------------------------------------------------------------------------------*/
function openNav() {
    document.getElementById("menu").style.width = "113px";
    document.getElementById("menu").style.zIndex = 10;
    isOpen = 1;
}
  
function closeNav() {
    document.getElementById("menu").style.width = "0";
    document.getElementById("menu").style.zIndex = -1;
    isOpen = 0;
}


/*-----------------------------------------------------------------------------------
Open news
-----------------------------------------------------------------------------------*/
function openNews() {
    disableSettings();
    enableWrapper();
    disableMainScreen();
    enableNews();
    disablePosts();
    disableChat();
    resetScroll();
    closeNav();
    currentpage = "news";
}


/*-----------------------------------------------------------------------------------
open posts
-----------------------------------------------------------------------------------*/
function openPosts(){
    disableNews();
    enablePosts();
    disableMainScreen();
    disableSettings();
    disableChat();
    enableWrapper();
    resetScroll();
    closeNav();
    msgOrPost = "posts";
    currentpage="posts";
}


/*-----------------------------------------------------------------------------------
Open Settings
-----------------------------------------------------------------------------------*/
function openSettings(){
    disableNews();
    disablePosts();
    disableChat();
    disableMainScreen();
    enableWrapper();
    enableSettings();
    resetScroll();
    closeNav();
    currentpage="settings";
}


/*-----------------------------------------------------------------------------------
Open Chat
-----------------------------------------------------------------------------------*/
function openChat(){
    disableNews();
    disablePosts();
    enableChat();
    disableMainScreen();
    enableWrapper();
    disableSettings();
    resetScroll();
    closeNav();
    msgOrPost = "msg";
    currentpage="chat";
}


/*-----------------------------------------------------------------------------------
Reset scroll
-----------------------------------------------------------------------------------*/
function resetScroll() {
    el.scrollLeft = 0;
    el.scrollTop = 0;
}


/*-----------------------------------------------------------------------------------
Disable/Enable main screen
-----------------------------------------------------------------------------------*/
function disableMainScreen() {
    document.getElementsByClassName("mainscreen")[0].style.display = "none";
}

function enableMainScreen() {
    document.getElementsByClassName("mainscreen")[0].style.display = "block";
}


/*-----------------------------------------------------------------------------------
Disable/Enable settings
-----------------------------------------------------------------------------------*/
function disableSettings() {
    document.getElementsByClassName("settings")[0].style.display = "none";
    document.getElementsByClassName("help")[0].style.display = "none";
}

function enableSettings(){
    document.getElementsByClassName("settings")[0].style.display = "block";
    document.getElementsByClassName("help")[0].style.display = "none";
}


/*-----------------------------------------------------------------------------------
Disable/Enable wrapper
-----------------------------------------------------------------------------------*/
function disableWrapper(){
    document.getElementsByClassName("scrollable")[0].style.display = "none";
}

function enableWrapper(){
    document.getElementsByClassName("scrollable")[0].style.display = "block";
}