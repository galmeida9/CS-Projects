window.onclick = function(event) {
    var post = document.getElementById("viewPost");
    var confirmation = document.getElementById("confirmationPopup");
    var chat = document.getElementById("personPopup");
    var sharePost = document.getElementById("personPopupPost");
    var shareperson = document.getElementById("confirmationPopupShare");

    if (event.target == filterpopup) {
        filterpopup.style.display = "none";
        currentpage = "news";
    }

    else if (event.target == post) {
        post.style.display = "none";
        document.getElementById("viewVideoLarge").pause();
        document.querySelectorAll(".postedVideo").forEach(function(element) {
            element.play();
        });
        currentpage = "posts";
    }

    else if (event.target == confirmation) {
        confirmation.style.display = "none";
        currentpage = "selectPost";
    }
    else if (event.target == chat) {
        chat.style.display = "none";
        currentpage = "chat";
    }
    else if (event.target == sharePost) {
        sharePost.style.display = "none";
        if (lastpage == "posts") currentpage = "posts";
        else currentpage = "newsStories";
    }
    else if (event.target == shareperson) {
        shareperson.style.display = "none";
        currentpage = "sharePost";
    }
}