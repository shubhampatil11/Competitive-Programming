/**Loops in MO's Algo**/
int L = -1, R = 0;
for(int i = 0; i < q; i++) {
    while(qq[i].l > L) del(L++);
    while(qq[i].l < L) ins(--L);
    while(qq[i].r > R) ins(++R);
    while(qq[i].r < R) del(R--);
}
