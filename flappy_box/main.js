/* Author: Wonhyuk Choi */
/* Some TypeScript for the flappy box game. */
// GAME PROPERTIES
var GAME_HEIGHT = 270;
var GAME_WIDTH = 480;
var FPS = 1 / 20;
// PIPE PROPERTIES
var PIPE_WIDTH = 10;
var PIPE_COLOR = "green";
var PIPE_FREQ = 150; // Controls how often pipes are generated
// BOX PROPERTIES
var BOX_COLOR = "red";
var BOX_SIZE = 30;
var BOX_INIT_X = 10;
var BOX_INIT_Y = GAME_HEIGHT - 120;
var GRAVITY = -0.05;
// SCORE PROPERTIES
var SCORE_SIZE = "20px";
var SCORE_FONT = "Consolas";
var SCORE_COLOR = "red";
var SCORE_X = 369;
var SCORE_Y = 248;
// Global variables
var birdBox;
var pipes;
var score;
// Buttons
var flyButton = document.querySelector("#fly"), resetButton = document.querySelector("#reset");
flyButton.addEventListener("click", fly);
resetButton.addEventListener("click", startGame);
function fly() {
    birdBox.fly();
}
// Intialize the game
function startGame() {
    birdBox = new Bird(BOX_SIZE, BOX_SIZE, BOX_COLOR, BOX_INIT_X, BOX_INIT_Y);
    pipes = new Pipes();
    score = new Score(SCORE_SIZE, SCORE_FONT, SCORE_COLOR, SCORE_X, SCORE_Y);
    gameArea.start();
}
// The game playing field
var gameArea = {
    canvas: document.createElement("canvas"),
    // Must clear & draw canvas on each frame iteration
    start: function () {
        this.canvas.width = GAME_WIDTH;
        this.canvas.height = GAME_HEIGHT;
        this.context = this.canvas.getContext("2d");
        var gameBox = document.querySelector(".gameBox");
        gameBox.insertBefore(this.canvas, gameBox.childNodes[0]);
        this.frame = 0;
        clearInterval(this.interval); // Needed for resetting
        this.interval = setInterval(updateGame, 1 / FPS);
    },
    clear: function () {
        this.context.clearRect(0, 0, this.canvas.width, this.canvas.height);
    }
};
// Base class applicable for both pipes & box
// changed y position to be (0,0) in the bottom left corner
function BaseItem(width, height, color, x, y) {
    this.width = width;
    this.height = height;
    this.x = x;
    this.y = y;
    this.color = color;
    this.show = function () {
        context = gameArea.context;
        var xPos = this.x, yPos = GAME_HEIGHT - this.y;
        context.fillStyle = this.color;
        context.fillRect(xPos, yPos, this.width, this.height);
    };
}
// The "bird", which is really a box
// Gravitates down each frame
function Bird(width, height, color, x, y) {
    BaseItem.call(this, width, height, color, x, y);
    this.acceleration = 0;
    this.velocity = 0;
    // For simpler API
    this.update = function () {
        this.updatePos();
        this.show();
    };
    this.updatePos = function () {
        this.gravitate();
        this.y += this.velocity;
    };
    this.gravitate = function () {
        this.velocity += GRAVITY;
    };
    this.fly = function () {
        this.velocity += 1;
    };
    this.isOutOfBounds = function () {
        // If box hits the bottom of playing field
        return (this.y <= BOX_SIZE || this.y >= GAME_HEIGHT);
    };
}
// Score text in upper right corner
function Score(size, font, color, x, y) {
    this.size = size;
    this.font = font;
    this.color = color;
    this.xPos = x;
    this.yPos = GAME_HEIGHT - y;
    this.update = function () {
        var scoreText = "Score: " + gameArea.frame.toString();
        context = gameArea.context;
        context.font = this.size + " " + this.font;
        context.fillStyle = this.color;
        context.fillText(scoreText, this.xPos, this.yPos);
    };
}
// This class has an array that holds pairs of pipes 
function Pipes() {
    var topIdx = 0, botIdx = 1;
    this.pipeArray = [newPipes()];
    this.update = function () {
        // If leftmost pipe is out of bounds, remove it
        if (this.pipeArray[0][topIdx].x - 1 < -(PIPE_WIDTH)) {
            this.pipeArray = this.pipeArray.slice(1);
        }
        for (i = 0; i < this.pipeArray.length; i++) {
            // Update positions
            --this.pipeArray[i][topIdx].x;
            --this.pipeArray[i][botIdx].x;
            // Show each pipe
            this.pipeArray[i][topIdx].show();
            this.pipeArray[i][botIdx].show();
        }
    };
    this.hasCrashed = function () {
        for (i = 0; i < this.pipeArray.length; i++) {
            var boxX = birdBox.x, pipeX = this.pipeArray[i][0].x, boxPassed = boxX > pipeX, notInRange = boxX + BOX_SIZE < pipeX;
            if (boxPassed || notInRange) {
                continue;
            }
            var topPipe = this.pipeArray[i][topIdx], botPipe = this.pipeArray[i][botIdx], topPipeBottom = topPipe.y - topPipe.height, botPipeTop = botPipe.y;
            var boxTop = birdBox.y, boxBot = birdBox.y - BOX_SIZE;
            var hitTopPipe = boxTop > topPipeBottom, hitBotPipe = boxBot < botPipeTop;
            if (hitTopPipe || hitBotPipe) {
                return true;
            }
        }
        return false;
    };
}
// Generate a pair of new pipes on the rightmost of playing field
function newPipes() {
    var minHeight = 20, maxHeight = 200, minGap = 50, maxGap = 200;
    function randomInt(min, max) {
        var randomVal = Math.random() * (max - min) + min;
        return Math.floor(randomVal);
    }
    var bottomHeight = randomInt(minHeight, maxHeight), gap = randomInt(minGap, maxGap);
    var bottomX = gameArea.canvas.width, bottomY = bottomHeight, topX = bottomX, topY = GAME_HEIGHT, topHeight = GAME_HEIGHT - bottomHeight - gap;
    var bottom = new BaseItem(PIPE_WIDTH, bottomHeight, PIPE_COLOR, bottomX, bottomY), top = new BaseItem(PIPE_WIDTH, topHeight, PIPE_COLOR, topX, topY);
    return [top, bottom];
}
function updateGame() {
    // Inefficient to loop through pipes twice, but so be it
    if (pipes.hasCrashed() || birdBox.isOutOfBounds()) {
        return;
    }
    // Update frames
    gameArea.clear();
    ++gameArea.frame;
    var canSpawnPipe = (gameArea.frame % PIPE_FREQ) === 0;
    // Update game
    if (canSpawnPipe) {
        pipes.pipeArray.push(newPipes());
    }
    pipes.update();
    birdBox.update();
    score.update();
}
