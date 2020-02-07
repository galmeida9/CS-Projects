var modal = document.getElementById("myModal");

function showImg(img) {
    var modalImg = document.getElementById("img01");
    modal.style.display = "block";
    modalImg.src = img;
}

var span = document.getElementsByClassName("close")[0];

span.onclick = function() { 
    modal.style.display = "none";
}