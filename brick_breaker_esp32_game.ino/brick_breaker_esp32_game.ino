#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// --- OLED ---
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// --- Joystick ---
#define JOY_X  34
#define JOY_Y  35
#define JOY_SW 15

// --- LEDs ---
#define LED_GREEN 26
#define LED_RED   27

// --- Paddle ---
#define PADDLE_W 20
#define PADDLE_H  3
#define PADDLE_Y 58
int paddleX;

// --- Ball ---
float ballX, ballY;
float ballDX, ballDY;
#define BALL_SIZE 3

// --- Bricks ---
#define BRICK_COLS  8
#define BRICK_ROWS  3
#define BRICK_W    14
#define BRICK_H     4
#define BRICK_GAP   2
#define BRICK_OFFSET_X 4
#define BRICK_OFFSET_Y 4
bool bricks[BRICK_ROWS][BRICK_COLS];
int bricksLeft;

// --- Game state ---
int score;
int lives;
bool gameOver;
bool gameWon;

void resetBricks() {
  bricksLeft = 0;
  for (int r = 0; r < BRICK_ROWS; r++)
    for (int c = 0; c < BRICK_COLS; c++) {
      bricks[r][c] = true;
      bricksLeft++;
    }
}

void resetBall() {
  ballX = SCREEN_WIDTH / 2;
  ballY = SCREEN_HEIGHT / 2;
  ballDX = 1.5;
  ballDY = -1.5;
}

void resetGame() {
  paddleX = (SCREEN_WIDTH - PADDLE_W) / 2;
  score = 0;
  lives = 3;
  gameOver = false;
  gameWon = false;
  resetBricks();
  resetBall();
  digitalWrite(LED_RED, LOW);
  digitalWrite(LED_GREEN, LOW);
}

void setup() {
  pinMode(LED_GREEN, OUTPUT);
  pinMode(LED_RED, OUTPUT);
  pinMode(JOY_SW, INPUT_PULLUP);

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    while (true);
  }

  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
  display.setTextSize(1);
  display.setCursor(18, 16);
  display.println("BRICK BREAKER");
  display.setCursor(12, 32);
  display.println("Tilt to move pad");
  display.setCursor(22, 46);
  display.println("Lives: 3");
  display.display();
  delay(2500);

  randomSeed(analogRead(0));
  resetGame();
}

void readJoystick() {
  int x = analogRead(JOY_X);
  // Move paddle left/right
  if (x < 1000) paddleX -= 4;
  if (x > 3000) paddleX += 4;
  // Clamp paddle inside screen
  paddleX = constrain(paddleX, 0, SCREEN_WIDTH - PADDLE_W);
}

void moveBall() {
  ballX += ballDX;
  ballY += ballDY;

  // Wall bounce left/right
  if (ballX <= 0) { ballX = 0; ballDX = abs(ballDX); }
  if (ballX >= SCREEN_WIDTH - BALL_SIZE) { ballX = SCREEN_WIDTH - BALL_SIZE; ballDX = -abs(ballDX); }

  // Wall bounce top
  if (ballY <= 0) { ballY = 0; ballDY = abs(ballDY); }

  // Ball fell below paddle
  if (ballY > SCREEN_HEIGHT) {
    lives--;
    if (lives <= 0) {
      gameOver = true;
      digitalWrite(LED_RED, HIGH);
    } else {
      // Flash red briefly
      digitalWrite(LED_RED, HIGH);
      delay(200);
      digitalWrite(LED_RED, LOW);
      resetBall();
    }
    return;
  }

  // Paddle bounce
  if (ballY + BALL_SIZE >= PADDLE_Y &&
      ballY + BALL_SIZE <= PADDLE_Y + PADDLE_H + 2 &&
      ballX + BALL_SIZE >= paddleX &&
      ballX <= paddleX + PADDLE_W) {
    ballDY = -abs(ballDY);
    // Angle based on where ball hits paddle
    float hitPos = (ballX - paddleX) / (float)PADDLE_W; // 0.0 to 1.0
    ballDX = (hitPos - 0.5) * 4.0;
    if (ballDX == 0) ballDX = 0.5;
  }

  // Brick collision
  for (int r = 0; r < BRICK_ROWS; r++) {
    for (int c = 0; c < BRICK_COLS; c++) {
      if (!bricks[r][c]) continue;

      int bx = BRICK_OFFSET_X + c * (BRICK_W + BRICK_GAP);
      int by = BRICK_OFFSET_Y + r * (BRICK_H + BRICK_GAP);

      if (ballX + BALL_SIZE >= bx && ballX <= bx + BRICK_W &&
          ballY + BALL_SIZE >= by && ballY <= by + BRICK_H) {
        bricks[r][c] = false;
        bricksLeft--;
        score += 10;
        ballDY = -ballDY;

        // Flash green
        digitalWrite(LED_GREEN, HIGH);
        delay(30);
        digitalWrite(LED_GREEN, LOW);

        if (bricksLeft == 0) {
          gameWon = true;
          digitalWrite(LED_GREEN, HIGH);
        }
        return;
      }
    }
  }
}

void drawGame() {
  display.clearDisplay();

  // Draw bricks
  for (int r = 0; r < BRICK_ROWS; r++) {
    for (int c = 0; c < BRICK_COLS; c++) {
      if (!bricks[r][c]) continue;
      int bx = BRICK_OFFSET_X + c * (BRICK_W + BRICK_GAP);
      int by = BRICK_OFFSET_Y + r * (BRICK_H + BRICK_GAP);
      display.fillRect(bx, by, BRICK_W, BRICK_H, SSD1306_WHITE);
    }
  }

  // Draw paddle
  display.fillRoundRect(paddleX, PADDLE_Y, PADDLE_W, PADDLE_H, 1, SSD1306_WHITE);

  // Draw ball
  display.fillRect((int)ballX, (int)ballY, BALL_SIZE, BALL_SIZE, SSD1306_WHITE);

  // Score and lives
  display.setTextSize(1);
  display.setCursor(0, 57);
  display.print(score);
  display.setCursor(100, 57);
  display.print("L:");
  display.print(lives);

  display.display();
}

void showGameOver() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(25, 14);
  display.println("GAME OVER!");
  display.setCursor(28, 30);
  display.print("Score: ");
  display.println(score);
  display.setCursor(8, 48);
  display.println("Push btn to retry");
  display.display();

  while (digitalRead(JOY_SW) == HIGH) delay(50);
  digitalWrite(LED_RED, LOW);
  resetGame();
}

void showYouWin() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(28, 14);
  display.println("YOU WIN!!");
  display.setCursor(28, 30);
  display.print("Score: ");
  display.println(score);
  display.setCursor(8, 48);
  display.println("Push btn to play");
  display.display();

  while (digitalRead(JOY_SW) == HIGH) delay(50);
  digitalWrite(LED_GREEN, LOW);
  resetGame();
}

void loop() {
  if (gameOver) { showGameOver(); return; }
  if (gameWon)  { showYouWin();  return; }

  readJoystick();
  moveBall();
  drawGame();

  delay(16); // ~60fps feel
}