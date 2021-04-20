%Tinh toan bien ap xung:
%Cac thong so can thiet
U1 = 5;             %(V)
U2 = 15;            %(V)
I2 = 50*10^(-3);    %50mA
f = 400000;          %(Hz)
eff = 0.8;           %efficiency 80%
S = 0.045*0.045;    %(cm2) dien tich loi bien ap
B = 10000;           %(Gauss)
J = 2;               %(A/mm2): mat do dong dien an toan

%Tinh dong so cap:
P2 = U2*I2;
P1 = ceil(P2/eff);      %roundup
I1 = P1/U1;

%Tinh kich thuoc day quan:
d1 = 2*sqrt(I1/(J*pi));
d2 = 2*sqrt(I2/(J*pi));

%Tinh so vong day can quan:
N = 10^8/(4.44*S*f*B);
n1 = U1*N;
n2 = U2*N;

fprintf('d1 = %5.2fmm, d2 = %5.2fmm\n', d1, d2);
fprintf('n1 = %d vong, n2 = %d vong', round(n1), round(n2));