// Expected output:
// 10! = 3628800
//

int num = 10;

void main() {
    int res = 1;
    int i;
    for (i = 1; i <= num; i = i + 1) {
        res = res * i;
    }
    print(num, "! = ", res);
}
