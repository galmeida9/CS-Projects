var nightmodeOn = 0;

/*-------------------------------------------------------------------------------------
Toggle night mode
--------------------------------------------------------------------------------------*/
function nightmode() {
    var i;
    if (nightmodeOn == 0) {
        document.querySelectorAll(".darkmodeText").forEach(function(element) {
            element.style.color = 'white';
        });
        document.querySelectorAll(".darkmodeBackground").forEach(function(element) {
            element.style.background = 'black';
        });
        document.getElementById("filter").src = "resources/filter_white.png";
        document.getElementById("nightmodeIcon").src = "resources/night_mode_on.png";
        for (i = 0; i < numberOfCategories; i++) {
            document.getElementsByClassName("newsCheckmark")[i].src = "resources/check_white.png";
        }
        document.querySelectorAll(".sharePost").forEach(function(element) {
            element.src="resources/share_white.png";
        });
        document.querySelectorAll(".userdarkmode").forEach(function(element) {
            element.src="resources/user_white.png";
        });
        document.getElementById("addPostImg").src="resources/plus_white.png";
        document.getElementById("addChat").src="resources/plus_white.png";
        document.getElementById("selectImg").src="resources/image_white.png";
        document.getElementById("selectVideo").src="resources/video_white.png";
        document.getElementById("selectLocation").src="resources/location_white.png";
        document.getElementById("selectMicrophone").src="resources/microphone_white.png";
        document.getElementById("microphoneGIF").src="resources/microphone_animation_dark.gif";
        document.getElementById("shareNews").src="resources/share_white.png";
        document.getElementById("emoji").src="resources/emoji_white.png";
        document.getElementById("backButtonHelp").src="resources/back.png";
        document.getElementById("menuButtonHelp").src="resources/menu.png";
        nightmodeOn = 1;
    }

    else if (nightmodeOn == 1) {
        document.querySelectorAll(".darkmodeText").forEach(function(element) {
            element.style.color = 'black';
        });
        document.querySelectorAll(".darkmodeBackground").forEach(function(element) {
            element.style.background = 'white';
        });
        document.getElementById("filter").src = "resources/filter.png";
        for (i = 0; i < numberOfCategories; i++) {
            document.getElementsByClassName("newsCheckmark")[i].src = "resources/check.png";
        }
        document.querySelectorAll(".userdarkmode").forEach(function(element) {
            element.src="resources/user.png";
        });
        document.querySelectorAll(".sharePost").forEach(function(element) {
            element.src="resources/share.png";
        });
        document.getElementById("addPostImg").src="resources/plus.png";
        document.getElementById("addChat").src="resources/plus.png";
        document.getElementById("selectImg").src="resources/image.png";
        document.getElementById("selectVideo").src="resources/video.png";
        document.getElementById("selectLocation").src="resources/location.png";
        document.getElementById("selectMicrophone").src="resources/microphone.png";
        document.getElementById("nightmodeIcon").src = "resources/night_mode_off.png";
        document.getElementById("microphoneGIF").src="resources/microphone_animation.gif";
        document.getElementById("shareNews").src="resources/share.png";
        document.getElementById("emoji").src="resources/emoji.png";
        document.getElementById("backButtonHelp").src="resources/back.png";
        document.getElementById("menuButtonHelp").src="resources/menu.png";
        nightmodeOn = 0;
    }
}