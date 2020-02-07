// Variables

var scene, renderer, currCamera, viewSize = 4/5;
var topCamera, perCamera, ballCamera, camera;
var controls;

var cannons = [], balls = [], wallSurface;
var cannonRadius = 5, cannonLength = 50, ballRadius = 5, wallLength = 100, wallHeight = 4*ballRadius;

var wallsColor = 0xff0000, ballsColor = 0xffffff, cannonsColor = 0x00ff00, selectedColor = 0x0000ff, backgroundColor = 0x000000;
var materials = [], leftLimit, rightLimit, distanceCannonWall = 100;
var wireframeOn = true, showAxis = true;
var nullVector = new THREE.Vector3(0, 0, 0);

var width = window.innerWidth, height = window.innerHeight, oldWidth = width, oldHeight = height;
var timePrev = 0;
var deacceleration = 995/1000;

let COR = 0.8, stopVelocity = 0.0001

var movementFlags = {"moveLeft":0, "moveRight":0, "shooting": 0}, selectedCannon;
var angleMovement = Math.PI/360;

// Classes

class Cannon extends THREE.Object3D {
    constructor(x, y, z, angle) {
        super();

        this.position.set(x, y+cannonRadius, z);
        var geometry = new THREE.CylinderGeometry(cannonRadius, cannonRadius*1.5, cannonLength, 64, 1);
        var material = new THREE.MeshBasicMaterial({color: cannonsColor, wireframe: true});
        materials.push(material);
        var mesh = new THREE.Mesh(geometry, material);
        this.add(mesh);
        mesh.rotateX(-Math.PI/2);
        this.rotateY(angle);
        this.mesh = mesh;
        this.canShoot = true;
        this._direction = new THREE.Vector3(Math.sin(this.rotation.y),0,1);
    }

    getDirection() {
        return new THREE.Vector3(this._direction.x, this._direction.y, this._direction.z)
    }

    moveLeft(leftLimit, delta) {
        var aux = angleMovement*delta;
        var flag = (this.position.x -(Math.tan(this.rotation.y+aux)*distanceCannonWall) ) >= leftLimit
        if (flag) {
            this.rotateY(aux);
            this._direction.set(Math.sin(this.rotation.y+aux), 0, Math.cos(this.rotation.y));
        }
    }

    moveRight(rightLimit, delta) {
        var aux = -angleMovement*delta;
        var flag = (this.position.x -(Math.tan(this.rotation.y+aux)*distanceCannonWall) ) <= rightLimit
        if (flag) {
            this.rotateY(aux);
            this._direction.set(Math.sin(this.rotation.y), 0, Math.cos(this.rotation.y));
        }
    }

    changeColor(color) {
        this.mesh.material.color.setHex(color);
    }

    changeShooting(value) {
        this.canShoot = value;
    }

    getRotY() {
        return this.rotation.y;
    }

    getLaunchPosition() {
        return this.getDirection().multiplyScalar(-cannonLength/2).add(this.position);
    }
}

class Ball extends THREE.Object3D {
    constructor() {
        super();
        var geometry = new THREE.SphereGeometry( ballRadius, 16, 16);
        var material = new THREE.MeshBasicMaterial({ color:0xffffff, wireframe: wireframeOn });
        materials.push(material);
        this.mesh = new THREE.Mesh(geometry, material);
        this.mesh.setRotationFromEuler(this.mesh.rotation);
        this.axis = new THREE.AxesHelper(2*ballRadius);
        this.axis.visible = showAxis;
        this.mesh.add(this.axis);
        this.add(this.mesh);
        this.velocity = nullVector.clone();
        this._direction = new THREE.Vector3(0,0,0);
        this.angle = 0; this.rotX = 0;
    }

    setPosition(x, y, z) {
        this.position.set(x, y, z);
    }

    getPosition() {
        return this.position;
    }

    positionIncrease(delta) {
        var vector = this.velocity.clone().multiplyScalar(delta)
        if (vector.length()) this.position.add(vector);
    }

    increaseRotation(delta) {
        var rot = Math.abs(this.getVelocity().length()) / (ballRadius*2) * delta;
        if (rot) this.mesh.rotateX(rot);
    }

    setVelocity(x, y, z) {
        this.velocity = new THREE.Vector3(x, y, z);
        this._direction = this.velocity.clone().normalize();
        this.angle = getAngle(this._direction);
        this.rotation.y = (this.angle);
    }

    getDirection() {
        return this._direction.clone();
    }

    changeVelocityScalar(scalar, delta) {
        if (this.velocity != nullVector) this.velocity.multiplyScalar(scalar**delta);

        if (Math.abs(this.velocity.x) < stopVelocity
            && Math.abs(this.velocity.z) < stopVelocity) {
            this.velocity = nullVector;
        }
    }

    getVelocity() {
        return this.velocity.clone();
    }

    setAxis(value) {
        this.axis.visible = value;
    }
}

class Walls extends THREE.Object3D {
    constructor(x, y, z) {
        super();
        var size = wallLength, heightSize = wallHeight;
        this.position.set(x, y, z-size);

        var material = new THREE.MeshBasicMaterial({color: wallsColor, wireframe: true});
        materials.push(material);
        var geometry = new THREE.BoxGeometry(2, heightSize, 2*size, 8, 8);

        //Left wall
        var meshLeft = new THREE.Mesh(geometry, material);
        meshLeft.position.set(x-size,y+heightSize/2,z);
        leftLimit = x - size;

        //Right wall
        var meshRight = new THREE.Mesh(geometry, material);
        meshRight.position.set(x+size,y+heightSize/2,z);
        rightLimit = x + size;

        //Center wall
        var meshCenter = new THREE.Mesh(geometry, material);
        meshCenter.position.set(x,y+heightSize/2,z-size);
        meshCenter.rotateY(Math.PI/2);

        this.add(meshLeft);
        this.add(meshRight);
        this.add(meshCenter);
    }
}

class OrtCamera extends THREE.OrthographicCamera {
    constructor(x, y, z, lookx, looky, looz) {
        super(width / - 2 * viewSize, width / 2 * viewSize, height / 2 * viewSize, height / - 2 * viewSize, 1, 800);

        this.position.x = x;
        this.position.y = y;
        this.position.z = z;
        this.lookAt(lookx,looky,looz);
    }
}

class PerCamera extends THREE.PerspectiveCamera {
    constructor(x, y, z, lookx, looky, looz) {
        super(80, window.innerWidth / window.innerHeight, 1, 1000);

        this.position.x = x;
        this.position.y = y;
        this.position.z = z;
        this.lookAt(lookx,looky,looz);
    }

    setCameraPosition(x, y, z) {
        this.position.x = x;
        this.position.y = y;
        this.position.z = z;
    }

    setFollowingBall(ball) {
        this.ball = ball;
    }

    getFollowingBall() {
        return this.ball;
    }

    setCameraRotation(value) {
        this.rotation.y = value;
    }

    lookAtObject(pos) {
        this.lookAt(pos);
    }
}

// Create all things

function createcannon(x, y, z, angle, tag) {
    'use strict';
    var cannon = new Cannon(x, y, z, angle);
    cannons[tag] = cannon;
    scene.add(cannon);
}

function createBall() {
    var ball = new Ball();
    var pI = selectedCannon.getLaunchPosition();
    ball.setPosition(pI.x, pI.y, pI.z);
    var random = 2 + Math.random();
    ball.setVelocity( -selectedCannon.getDirection().x * random, 0, -selectedCannon.getDirection().z * random);
    
    balls.push(ball);
    selectedCannon.changeShooting(false);
    var cannonToChange = selectedCannon;
	window.setTimeout(function() { cannonToChange.changeShooting(true) }, 500);
    scene.add(ball);
}

function createFieldBalls(numb, coorX, coorZ) {
    for (var i = 0; i < Math.random() * numb; i++) {
        var ball = new Ball();
        var x = Math.random() * coorX;
        var z = Math.random() * coorZ;
        var onTop = true
        while (onTop) {
            var j = 0;
            var newBallPos = new THREE.Vector3(x, 10, z);
            for (j = 0; j < balls.length; j++) {
                var ballPos = new THREE.Vector3(balls[j].position.x, balls[j].position.y, balls[j].position.z);
                if (ballPos.distanceTo(newBallPos) < 20) {
                    x = Math.random() * coorX;
                    z = Math.random() * coorZ;
                    break;
                }
            }

            if (j == balls.length) onTop = false;
        }
        ball.setPosition(x, ballRadius, z);
        balls.push(ball);
        scene.add(ball);
    }
}

function createWalls(x, y, z) {
    'use strict';
    wallSurface = new Walls(x, y, z);
    scene.add(wallSurface);
}

function createPerspectiveCamera() {
    'use strict';

    camera = new THREE.PerspectiveCamera(80, window.innerWidth / window.innerHeight, 1, 1000);

    camera.position.x = 50;
    camera.position.y = 50;
    camera.position.z = 50;
    camera.lookAt(0,0,0);
}

function createScene() {
    'use strict';

    scene = new THREE.Scene();
    scene.add(new THREE.AxesHelper(100));

    //Plane for ground
    var geometry = new THREE.PlaneGeometry( 2*wallLength, 2*wallLength, 10, 10);
    var material = new THREE.MeshBasicMaterial( {color: 0x808080, side: THREE.DoubleSide} );
    var plane = new THREE.Mesh( geometry, material );
    plane.rotateX( - Math.PI / 2);
    plane.position.set(0, 0, -wallLength);
    scene.add( plane );

    //Creation of Models
    createcannon(50, 0, distanceCannonWall, Math.PI/16, "right");
    createcannon(0, 0, distanceCannonWall, 0, "center");
    createcannon(-50, 0, distanceCannonWall, -Math.PI/16, "left");
    selectedCannon = cannons["center"];
    selectedCannon.changeColor(selectedColor);

    createWalls(0,0,0);

    createFieldBalls(3, rightLimit - ballRadius, -(distanceCannonWall - ballRadius));
    createFieldBalls(3, leftLimit + ballRadius, -(distanceCannonWall - ballRadius));
}

// Event action

function onResize() {
    'use strict';
    renderer.setSize(window.innerWidth, window.innerHeight);
    var windowVector = new THREE.Vector3(0,0,0);
    renderer.getSize(windowVector);
    width = windowVector.x, height = windowVector.y;

    var angle = oldWidth / oldHeight;

    if (window.innerHeight > 0 && window.innerWidth > 0) {
    
        if (width != oldWidth) {
            if (width > oldWidth) viewSize /= (1.01 ** angle);
            else viewSize *= (1.01 ** angle);
        }

        if (height != oldHeight) {
            if (height > oldHeight) viewSize /= (1.01 ** angle);
            else viewSize *= (1.01 ** angle);
        }
        camera.aspect = width / height;
        camera.updateProjectionMatrix();

        topCamera.left = width / -2 * viewSize;
        topCamera.right = -topCamera.left;
        topCamera.top = height / 2 * viewSize;
        topCamera.bottom = -topCamera.top;
        topCamera.updateProjectionMatrix();

        if (width > oldWidth) perCamera.fov -= Math.atan(angle);
        else if (width < oldWidth) perCamera.fov += Math.atan(angle);
        perCamera.aspect = width / height;
        perCamera.updateProjectionMatrix();


        if (width > oldWidth) ballCamera.fov -= Math.atan(angle);
        else if (width < oldWidth) ballCamera.fov += Math.atan(angle);
        ballCamera.aspect = width / height;
        ballCamera.updateProjectionMatrix();
        renderer.setSize(width, height);
    }
    oldWidth = width; oldHeight = height;
}

function onKeyDown(e) {
    'use strict';

    switch(e.keyCode) {
        case 48: //0 - Default Camera

            currCamera = camera;
            break;
        case 49: //1 - Top Camera
            currCamera = topCamera;
            break;
        case 50: //2 - Lateral Camera
            currCamera = perCamera;
            break;
        case 51: //3 - Front Camera
            followBall(ballCamera);
            break;
        case 52: //4 - Wireframe toggle
            wireframeOn = !wireframeOn;
            for (var i = 0; i < materials.length; i++)
                materials[i].wireframe = wireframeOn;
            break;
        case 81: //q - Select left cannon
            selectedCannon.changeColor(cannonsColor);
            selectedCannon = cannons["left"];
            selectedCannon.changeColor(selectedColor);
            break;
        case 87: //w - Select center cannon
            selectedCannon.changeColor(cannonsColor);
            selectedCannon = cannons["center"];
            selectedCannon.changeColor(selectedColor);
            break;
        case 69: //e - Select right cannon
            selectedCannon.changeColor(cannonsColor);
            selectedCannon = cannons["right"];
            selectedCannon.changeColor(selectedColor);
            break;
        case 82: //r - show balls axis
            showAxis = !showAxis;
            for (var i = 0; i < balls.length ; i++)
                balls[i].setAxis(showAxis);
            break;
        case 37: // < - Move left
            movementFlags["moveLeft"] = 1;
            break;
        case 39: // > - Move right
            movementFlags["moveRight"] = 1;
            break;
        case 32: //space bar - shoot a ball
            if (selectedCannon.canShoot)
                movementFlags["shooting"] = 1;
            break;
        default:
            break;
    }
}

function onKeyUp(e) {
    'use strict';

    switch(e.keyCode) {
        case 37: // < - Move left
            movementFlags["moveLeft"] = 0;
            break;
        case 39: // > - Move right
            movementFlags["moveRight"] = 0;
            break;
        case 32:
            movementFlags["shooting"] = 0;
            break;
        default:
            break;
    }
}

// Updating status

function updatePosition(delta) {
    if (movementFlags["moveLeft"]) selectedCannon.moveLeft(leftLimit, delta);
    if (movementFlags["moveRight"]) selectedCannon.moveRight(rightLimit, delta);
    if (movementFlags["shooting"] && selectedCannon.canShoot) createBall();

    for (var i = 0; i < balls.length ; i++) {
        var currentBall = balls[i];

        // Check if the ball should be removed
        if (currentBall.getPosition().z > 0 && currentBall.getVelocity().z > 0) {
            balls.splice(i, 1);
            if (currCamera == ballCamera && currCamera.getFollowingBall() == currentBall) followBall(currCamera);
            scene.remove(currentBall);
        }

        // Update position
        currentBall.positionIncrease(delta);

        // Update velocity
        currentBall.changeVelocityScalar(deacceleration, delta);

        // Update rotation
        currentBall.increaseRotation(delta);
    }

    if (currCamera == ballCamera) {
        var ball = currCamera.getFollowingBall();
        var cameraPos = ball.getPosition().clone().sub(ball.getDirection().multiplyScalar(40));

        currCamera.setCameraPosition(cameraPos.x, cameraPos.y + 30, cameraPos.z);
        currCamera.lookAtObject(ball.getPosition());
    }
}

function followBall(camera) {
    var ball = balls[balls.length - 1];

    camera.setFollowingBall(ball);
    currCamera = camera;
}

function checkLimits() {
	for (var i = 0; i < balls.length; i++) {
        var d, currentBall = balls[i];
        var velocity = currentBall.getVelocity()
        var position = currentBall.getPosition()

        // Check for collision with another ball
		for (var j = i + 1; j < balls.length; j++) {
            var mag = (ballRadius * 2) ** 2 - distanceBalls(currentBall, balls[j])
			if (mag >= 0)
                computeBallIntersection(mag, currentBall, balls[j])
        }
        
        velocity = currentBall.getVelocity()		
        position = currentBall.getPosition()

        // Check for collision with a wall
        if ((d = position.x - leftLimit - ballRadius) < 0) {
            // Left wall collision
            currentBall.setVelocity(-velocity.x * COR, velocity.y, velocity.z)
            currentBall.setPosition(position.x - d, position.y, position.z)
        }

        else if ((d = position.x - rightLimit + ballRadius) > 0) {
            // Right wall collision
            currentBall.setVelocity(-velocity.x * COR, velocity.y, velocity.z)
            currentBall.setPosition(position.x - d, position.y, position.z)
        }

        else if ((d = position.z + 2 * wallLength - ballRadius) < 0) {
            // Center wall collision
            currentBall.setVelocity(velocity.x, velocity.y, -velocity.z * COR)
            currentBall.setPosition(position.x, position.y, position.z - d)
        }
    }
}

function computeBallIntersection(overlapMagnitude, b1, b2) {

    // Calculate new velocities
    var v1 = b1.getVelocity(), v2 = b2.getVelocity()
    var x1 = b1.getPosition(), x2 = b2.getPosition()
    
    var aux11 = v1.clone().sub(v2)
    var aux12 = x1.clone().sub(x2)
    var aux13 = aux11.clone().dot(aux12)
    var aux14 = aux12.length() ** 2;
    var aux15 = aux13 / aux14

    var aux21 = v2.clone().sub(v1)
    var aux22 = x2.clone().sub(x1)
    var aux23 = aux21.clone().dot(aux22)
    var aux24 = aux22.length() ** 2;
    var aux25 = aux23 / aux24

    var aux16 = aux12.multiplyScalar(aux15)
    var aux26 = aux22.multiplyScalar(aux25)

    var v1f = v1.sub(aux16).multiplyScalar(COR);
    var v2f = v2.sub(aux26).multiplyScalar(COR);

    b1.setVelocity(v1f.x, v1f.y, v1f.z);
    b2.setVelocity(v2f.x, v2f.y, v2f.z);

    // Calculate new positions
    var position, displacement
    var d = Math.sqrt(overlapMagnitude) / 2

    // Overlapping resolution is visually smoother if
    // each ball moves by half of the overlapping magnitude
    displacement = b1.getVelocity().normalize().multiplyScalar(d)
    position = b1.getPosition().add(displacement)
    b1.setPosition(position.x, position.y, position.z)

    displacement = b2.getVelocity().normalize().multiplyScalar(d)
    position = b2.getPosition().add(displacement)
    b2.setPosition(position.x, position.y, position.z)

    var foundNewPosition = true;
    do {
        for (var i = 0; i < balls.length; i++) {
            var currentBall = balls[i];
            var mag = (ballRadius * 2) ** 2 - distanceBalls(currentBall, b1);
            if (mag >= 0 && currentBall != b1) {
                foundNewPosition = false;
                displacement = b1.getVelocity().normalize().multiplyScalar(d)
                position = b1.getPosition().add(displacement)
                b1.setPosition(position.x, position.y, position.z)
                break
            }
        }
        if (i == balls.length) foundNewPosition = true;
    } while(!foundNewPosition);

    foundNewPosition = true;
    do {
        for (var i = 0; i < balls.length; i++) {
            var currentBall = balls[i];
            var mag = (ballRadius * 2) ** 2 - distanceBalls(currentBall, b2);
            if (mag >= 0 && currentBall != b2) {
                foundNewPosition = false;
                displacement = b2.getVelocity().normalize().multiplyScalar(d)
                position = b2.getPosition().add(displacement)
                b2.setPosition(position.x, position.y, position.z)
                break
            }
        }
        if (i == balls.length) foundNewPosition = true;
    } while(!foundNewPosition);
}

// Core functions

function animate(time) {
    'use strict';
    var delta = (time - timePrev) / 10;

    controls.update();
    updatePosition(delta);
    checkLimits();
    render();

    timePrev = time;
    requestAnimationFrame(animate);
}

function render() {
    'use strict';
    renderer.render(scene, currCamera);
}

function init() {
    'use strict';

    renderer = new THREE.WebGLRenderer({antialias: true});
    renderer.setSize(width, height);
    renderer.setClearColor(new THREE.Color(backgroundColor))

    document.body.appendChild(renderer.domElement);

    createScene();
    createPerspectiveCamera();
    topCamera = new OrtCamera(0, 100, 0, 0, 0, 0);
    perCamera = new PerCamera(0, 200, 100, 0, 0, 0);
    ballCamera = new PerCamera(0, 30, 200, 0, 0, 0);

    currCamera = topCamera;
    render();

    controls = new THREE.OrbitControls(camera, renderer.domElement);
    controls.update();

    window.addEventListener("resize", onResize);
    window.addEventListener("keydown", onKeyDown);
    window.addEventListener("keyup", onKeyUp);
}

// Auxiliar functions

function collisionAngle(ball1, ball2) {
    return Math.acos(ball1.getDirection().dot(ball2.getDirection()))
}

function distanceBalls(thisBall, otherBall) {
	return (thisBall.getPosition().x - otherBall.getPosition().x) ** 2
	 + (thisBall.getPosition().z - otherBall.getPosition().z) ** 2
}

function getAngle(vector){
    var x = vector.x, z = vector.z;
    return Math.atan2(x, z);
}
