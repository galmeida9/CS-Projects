var el;
var drag = false;
var startx = 0;
var starty = 0;
var diffx = 0;
var diffy = 0;
var mouseMovement = 0;

if ("scrollable") {
    el = document.getElementById("scrollable");
} else {
    if (isIE || isFirefox) {
        el = document.documentElement;
    } else {
        el = document.body;
    }
}

function addEvent(name, el, func) {
    if (el.addEventListener) {
        el.addEventListener(name, func, false);
    } else if (el.attachEvent) {
        el.attachEvent('on' + name, func);
    } else {
        el[name] = func;
    }
}

addEvent('mousedown', el, onMouseDown);
addEvent('mousemove', el, onMouseMove);
addEvent('mouseup', el, onMouseUp);

function onMouseDown(e) {
    if (!e) { e = window.event; }
    if (e.target && e.target.nodeName === 'IMG') {
        e.preventDefault();
    } else if (e.srcElement && e.srcElement.nodeName === 'IMG') {
        e.returnValue = false;
    }
    startx = e.clientX + el.scrollLeft;
    starty = e.clientY + el.scrollTop;
    diffx = 0;
    diffy = 0;
    drag = true;
    mouseMovement = 0;
}

function onMouseMove(e) {
    if (drag === true) {
        if (!e) { e = window.event; }
        diffx = (startx - (e.clientX + el.scrollLeft));
        diffy = (starty - (e.clientY + el.scrollTop));
        el.scrollLeft += diffx;
        el.scrollTop += diffy;
        mouseMovement += diffy;
    }
}

function onMouseUp(e) {
    if (!e) { e = window.event; }
    drag = false;
    var start = 1,
        animate = function () {
            var step = Math.sin(start);
            if (step <= 0) {
                window.cancelAnimationFrame(animate);
            } else {
                el.scrollLeft += diffx * step;
                el.scrollTop += diffy * step;
                start -= 0.02;
                window.requestAnimationFrame(animate);
            }
        };
    animate();
}
