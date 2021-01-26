var scene, renderer;
var topCamera, lateralCamera, frontCamera;
var currCamera;
var viewSize = 1/4;

var robot, target;
var robotColor = 0xff0000;

var width = window.innerWidth;
var height = window.innerHeight;

var left = false, right = false, up = false, down = false;
var rotateBasePos = false, rotateBaseNeg = false, rotateArmPos = false, rotateArmNeg = false;

class Robot extends THREE.Object3D {
    constructor(width, height, depth, radius) {
        super();

        // Create robot base
        this.base = new RobotBase(width, height, depth, [[-10,20],[10,-20],[-10,-20],[10,20]]);
        this.add(this.base);

        // Create arm base
        this.armBase = new ArmBase(radius, 0, 3, 0);
        this.base.add(this.armBase);

        // Create robot arm
        this.robotArm = new RobotArm();
        this.armBase.add(this.robotArm);
        //this.movement = this.getWorldDirection(new THREE.Vector3());
    }

    getBase() {
        return this.base;
    }

    getArmBase() {
        return this.armBase;
    }

    getRobotArm() {
        return this.robotArm;
    }

    getMovement() {
        return this.movement;
    }

    moveLeft() {
        this.translateX(-1);
    }

    moveRight() {
        this.translateX(1);
    }

    moveUp() {
        this.translateZ(-1);
    }

    moveDown() {
        this.translateZ(1);
    }

    rotateBasePositive() {
        this.getArmBase().rotateY(0.1);
    }

    rotateBaseNegative() {
        this.getArmBase().rotateY(-0.1);
    }

    canRotateArmPos() {
        return this.getRobotArm().rotation.x <= Math.PI/4;
    }

    canRotateArmNeg() {
        return this.getRobotArm().rotation.x >= -Math.PI/4;
    }

    rotateArmPositive() {
        this.getRobotArm().rotateX(0.1);
    }

    rotateArmNegative() {
        this.getRobotArm().rotateX(-0.1);
    }
}

class RobotBase extends THREE.Object3D {
    constructor(width, height, depth, positions) {
        super();
        this.add(new THREE.Mesh(new THREE.BoxGeometry(width, height, depth, 1,1,1), new THREE.MeshBasicMaterial({color: robotColor, wireframe: true})));
        for (let i=0; i<4; i+=1){
            var geometry = new THREE.SphereGeometry( 5, 16, 16 );
            var material = new THREE.MeshBasicMaterial( {color: 0x797979, wireframe:true} );
            var sphere = new THREE.Mesh( geometry, material );

            sphere.position.set( positions[i][0], -8, positions[i][1]);
            this.add(sphere);
        }
    }
}

class ArmBase extends THREE.Object3D {
    constructor(radius, x, y, z) {
        super();
        var geometry = new THREE.SphereGeometry( radius, 16, 16, 0, 2*Math.PI, 0, 0.5*Math.PI );
        var material = new THREE.MeshBasicMaterial( {color: 0x797979, wireframe:true} );
        var armBase = new THREE.Mesh( geometry, material );
        armBase.position.set(x, y, z);
        this.add(armBase);
    }
}

class RobotArm extends THREE.Object3D {
    constructor() {
        super();

        // Create forearm
        this.foreArm = new Arm(3, 20, 3, 0, 16, 0);
        this.add(this.foreArm);

        // Create arm articulation
        this.articulation1 = new Articulation(3, 0, 12, 0);
        this.foreArm.add(this.articulation1);

        // Create arm
        this.arm = new Arm(3, 3, 30, 0, 0, -17);
        this.articulation1.add(this.arm);

        // Create hand articulation
        this.articulation2 = new Articulation(3, 0, 0, -15);
        this.arm.add(this.articulation2);

        // Create hand
        this.hand = new Hand(6, 6, 1, 0, 0, -3);
        this.articulation2.add(this.hand);
    }
}

class Arm extends THREE.Mesh {
    constructor(width, height, depth, x, y, z) {
        var geometry = new THREE.BoxGeometry( width, height, depth, 1,1,1);
        var material = new THREE.MeshBasicMaterial( {color: 0x797979, wireframe:true} );
        super(geometry, material);
        this.position.set(x, y, z);
    }
}

class Hand extends THREE.Mesh {
    constructor(width, height, depth, x, y, z) {
        var geometry = new THREE.BoxGeometry( width, height, depth, 1,1,1);
        var material = new THREE.MeshBasicMaterial( {color: 0x797979, wireframe:true} );
        super(geometry, material);
        this.position.set(x, y, z);

        var finger_positions = [-2,2];
        for (let i=0; i<2; i+=1){
            var geometry = new THREE.BoxGeometry( 1, 1, 5, 1,1,1);
            var material = new THREE.MeshBasicMaterial( {color: 0x797979, wireframe:true} );
            var finger = new THREE.Mesh( geometry, material );
            finger.position.set( 0, finger_positions[i], -2);
            this.add(finger);
        }
    }
}

class Articulation extends THREE.Mesh {
    constructor(radius, x, y, z) {
        var geometry = new THREE.SphereGeometry( radius, 16, 16 );
        var material = new THREE.MeshBasicMaterial( {color: 0x797979, wireframe:true} );
        super(geometry, material);
        this.position.set(x, y, z);
    }
}

class Target extends THREE.Object3D {
    constructor(cilRad1, cilRad2, cilHeight, cilx, cily, cilz, torusRad, torusx, torusy, torusz) {
        super();
        this.position.set(cilx,cily,cilz);
        this.add(new THREE.Mesh(new THREE.CylinderGeometry( cilRad1, cilRad2, cilHeight, 8 ), new THREE.MeshBasicMaterial({color: robotColor, wireframe: true})));

        // torus
        var geometry = new THREE.TorusGeometry(torusRad, 1, 16, 50);
        var material = new THREE.MeshBasicMaterial( {color: 0x797979, wireframe:true} );
        var torus = new THREE.Mesh( geometry, material );
        torus.position.set(torusx, torusy, torusz);
        this.add(torus);
    }
}

class Camera extends THREE.OrthographicCamera {
    constructor(x, y, z, lookx, looky, looz) {
        super(width / - 2 * viewSize, width / 2 * viewSize, height / 2 * viewSize, height / - 2 * viewSize, 1, 200);

        this.position.x = x;
        this.position.y = y;
        this.position.z = z;
        this.lookAt(lookx,looky,looz);
    }
}

function createRobot(x,y,z){ //Completed
    'use strict';
    robot = new Robot(30, 6, 50, 6);
    robot.position.set(x,y,z);
    scene.add(robot);
}

function createTarget(x, y, z) {
	'use strict';

    target = new Target(5, 5, 38, x, y, z, 4, 0, 24, 0);
    scene.add(target);
}

function createScene(){
    'use strict';

    scene = new THREE.Scene();
    scene.add(new THREE.AxesHelper(100));

    //Plane for help
    var geometry = new THREE.PlaneGeometry( 500, 500, 32 );
    var material = new THREE.MeshBasicMaterial( {color: 0xffffff, side: THREE.DoubleSide} );
    var plane = new THREE.Mesh( geometry, material );
    plane.rotateX( - Math.PI / 2);
    plane.position.set(0, 2, 0);
    scene.add( plane );

    //Creation of Models
    createRobot(0,15,0);
    createTarget(0, 21, -60);
}

function onResize(){
    'use strict';
    renderer.setSize(window.innerWidth, window.innerHeight);
    var width = renderer.getSize().width, height = renderer.getSize().height;

    if (window.innerHeight > 0 && window.innerWidth > 0) {
        topCamera.left = width / -2 * viewSize;
        topCamera.right = -topCamera.left;
        topCamera.top = height / 2 * viewSize;
        topCamera.bottom = -topCamera.top;
        topCamera.updateProjectionMatrix();
        renderer.setSize(width, height);

        lateralCamera.left = width / -2 * viewSize;
        lateralCamera.right = -topCamera.left;
        lateralCamera.top = height / 2 * viewSize;
        lateralCamera.bottom = -topCamera.top;
        lateralCamera.updateProjectionMatrix();
        renderer.setSize(width, height);

        frontCamera.left = width / -2 * viewSize;
        frontCamera.right = -topCamera.left;
        frontCamera.top = height / 2 * viewSize;
        frontCamera.bottom = -topCamera.top;
        frontCamera.updateProjectionMatrix();
        renderer.setSize(width, height);
    }

}

function onKeyDown(e){
    'use strict';

    switch(e.keyCode){
        case 49: //1 - Top Camera
            console.log("TopCamera activates!");
            currCamera = topCamera;
            break;
        case 50: //2 - Lateral Camera
            console.log("LateralCamera activates!");
            currCamera = lateralCamera;
            break;
        case 51: //3 - Front Camera
            console.log("FrontalCamera activates!");
            currCamera = frontCamera;
            break;
        case 52: //4 - Wireframe toggle
            scene.traverse(function (node){
                if (node instanceof THREE.Mesh){
                    node.material.wireframe = !node.material.wireframe;
                }
            });
            break;
        case 65: //a - Angle O1
            rotateBasePos = true;
            break;
        case 83: //s - Angle O1
            rotateBaseNeg = true;
            break;
        case 81: //q - Angle O2
            rotateArmPos = true;
            break;
        case 87: //w - Angle O2
            rotateArmNeg = true;
            break;
        case 37: // < - Move left
            left = true;
            break;
        case 39: // > - Move right
            right = true;
            break;
        case 38: // /\ - Move up
            up = true;
            break;
        case 40: // \/ - Move down
            down = true;
            break;
        default:
            console.log(e.keyCode);
            break;
    }
}

function onKeyUp(e){
    'use strict';

    switch(e.keyCode){
        case 65: //a - Angle O1
            rotateBasePos = false;
            break;
        case 83: //s - Angle O1
            rotateBaseNeg = false;
            break;
        case 81: //q - Angle O2
            rotateArmPos = false;
            break;
        case 87: //w - Angle O2
            rotateArmNeg = false;
            break;
        case 37: // < - Move left
            left = false;
            break;
        case 39: // > - Move right
            right = false;
            break;
        case 38: // /\ - Move up
            up = false;
            break;
        case 40: // \/ - Move down
            down = false;
            break;
        default:
            break;
    }
}

function moveRobot() {
    if (left) robot.moveLeft();
    if (right) robot.moveRight();
    if (up) robot.moveUp();
    if (down) robot.moveDown();
    if (rotateBasePos) robot.rotateBasePositive();
    if (rotateBaseNeg) robot.rotateBaseNegative();
    if (rotateArmPos && robot.canRotateArmPos()) robot.rotateArmPositive();
    if (rotateArmNeg  && robot.canRotateArmNeg()) robot.rotateArmNegative();
}

function animate(){
    'use strict';

    moveRobot();
    render();

    requestAnimationFrame(animate);
}

function render(){
    'use strict';
    renderer.render(scene, currCamera);
}

function init(){
    'use strict';

    renderer = new THREE.WebGLRenderer({antialias: true});
    renderer.setSize(width, height);

    document.body.appendChild(renderer.domElement);

    createScene();
    topCamera = new Camera(0, 100, 0, 0, 0, 0);
    lateralCamera = new Camera(50, 30, 0, 0, 30, 0);
    frontCamera = new Camera(0, 30, 50, 0, 30, 0);


    currCamera = topCamera;
    render();

    window.addEventListener("resize", onResize);
    window.addEventListener("keydown", onKeyDown);
    window.addEventListener("keyup", onKeyUp);
}
