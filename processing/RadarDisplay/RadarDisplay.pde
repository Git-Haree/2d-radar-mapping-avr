import processing.serial.*;

Serial port;
float angle = 0;
float distance = 0;

void setup() {
    size(800, 450);
    smooth();
    String portName = Serial.list()[0];
    port = new Serial(this, portName, 115200);
    port.bufferUntil('\n');
}

void draw() {
    background(0);
    drawRadarGrid();
    drawSweepLine();
    drawObstaclePoint();
}

void drawRadarGrid() {
    stroke(0, 200, 0);
    noFill();
    translate(width / 2, height - 50);

    for (int r = 50; r <= 350; r += 75) {
        arc(0, 0, r * 2, r * 2, PI, TWO_PI);
    }
    for (int a = 0; a <= 180; a += 30) {
        float rad = radians(a);
        line(0, 0, -cos(rad) * 350, -sin(rad) * 350);
    }
}

void drawSweepLine() {
    stroke(30, 255, 30, 180);
    float rad = radians(angle);
    line(0, 0, -cos(rad) * 350, -sin(rad) * 350);
}

void drawObstaclePoint() {
    if (distance > 0 && distance < 200) {
        float mapped = map(distance, 0, 200, 0, 350);
        float rad    = radians(angle);
        float px     = -cos(rad) * mapped;
        float py     = -sin(rad) * mapped;
        fill(255, 50, 50);
        noStroke();
        ellipse(px, py, 10, 10);
    }
}

void serialEvent(Serial p) {
    String raw = trim(p.readStringUntil('\n'));
    if (raw != null && raw.contains(",")) {
        String[] parts = split(raw, ',');
        if (parts.length == 2) {
            angle    = float(parts[0]);
            distance = float(parts[1]);
        }
    }
}
