// Expected output:
// 1
//

void main() {
    bool a = true;
    if (a) {
        bool a = false;
    }

    if (a) {
        print(a);
    }
    else {
        print("0");
    }
}
