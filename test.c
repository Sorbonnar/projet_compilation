// Test un programme complet

int a = 0, b = 200;

void main() {
    int i, c_lala = 5;

    for (i = 0; a / 2 < b * 10; i = i + 1) {
        a = a * 2;
        b = b % a;
        a = a + 1;

        if (a == 100) {
            a = a - 100;
        }
        else {
            a = a - 10;
        }
    }

    while (a < 100) {
        a = a + c_lala;
    }

    do {
        a = a - 1;
    } while (a > 100);


    b = 13;

    print("a: ", a, "b: ", b, "\n");

    // exit(); // Plus tard
}