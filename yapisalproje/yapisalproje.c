#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define KITAPLARCSV "/home/turabi/Desktop/yapisalproje/kitaplar.csv"
#define OGRENCILERCSV "/home/turabi/Desktop/yapisalproje/ogrenciler.csv"
#define KITAPODUNCCSV "/home/turabi/Desktop/yapisalproje/kitapodunc.csv"
#define KITAPYAZARCSV "/home/turabi/Desktop/yapisalproje/kitapyazar.csv"
#define YAZARLARCSV "/home/turabi/Desktop/yapisalproje/yazarlar.csv"
#define KITAPORNEKCSV "/home/turabi/Desktop/yapisalproje/kitapornek.csv"
typedef struct Yazar {
    int yazarID;
    char yazarAd[30];
    char yazarSoyad[30];
    struct Yazar *next;
} YAZAR;

typedef struct Ogrenci {
    char ogrID[9];
    char ad[30];
    char soyad[30];
    int puan;
    struct Ogrenci *next;
    struct Ogrenci *prev;
} OGRENCI;

typedef struct Kitap {
    char kitapAdi[30];
    char ISBN[14];
    int adet;
    struct Kitap *next;
    struct KitapOrnek *head;
} KITAP;

typedef struct KitapOrnek {
    char EtiketNo[20];
    char Durum[8];
    struct KitapOrnek *next;
} KORNEK;

typedef struct KitapYazar{
    char ISBN[14];
    int YazarID;
} KYAZAR;

typedef struct Tarih {
    int gun: 5;
    int ay: 4;
    int yil: 12;
}TARIH;

typedef struct KitapOdunc{
    char EtiketNo[20];
    char ogrID[9];
    int islemTipi:1;
    TARIH islemTarihi;
}KODUNC;

typedef struct KitapOduncListe {
    KODUNC *odunc;
    struct KitapOduncListe *next;
}KODUNCLISTE;

int savekitaplaroku(KITAP** headptr){
    FILE *kitap = fopen(KITAPLARCSV, "r+");
    FILE *kitapornek = fopen(KITAPORNEKCSV, "r+");
    char dummy[10];
    int i;
    if(fscanf(kitap,"%d")==-1){
        return -1;
    }
    fseek(kitap, 0, SEEK_SET);
    while(!feof(kitap)){
        KITAP *temp;
        temp = (KITAP*)malloc(sizeof(KITAP));
        fscanf(kitap,"%[^,],%[^,],%d\n",temp->kitapAdi,temp->ISBN,&(temp->adet));
        temp->head = NULL;
        for(i=0;i<temp->adet;i++) {
            KORNEK *kor = (KORNEK *) malloc(sizeof(KORNEK));
            fscanf(kitapornek, "%[^,],%[^\n]\n", kor->EtiketNo, kor->Durum);
            kor->next = temp->head;
            temp->head = kor;
        }
        temp->next = *headptr;
        *headptr = temp;

    }
    fclose(kitap);
    fclose(kitapornek);
}
KITAP* kitapbul(KITAP** headptr, char ISBN[13]){
    KITAP *temp;
    temp = *headptr;
    while (temp != NULL) {
        if(strcmp(temp->ISBN,ISBN) == 0){
            return temp;
        }
        temp = temp->next;
    }
    return NULL;
}
void kitapekle(KITAP** headptr){
    FILE *kitap = fopen(KITAPLARCSV, "r+");
    FILE *kitapornek = fopen(KITAPORNEKCSV, "r+");
    fseek(kitap, 0, SEEK_END);
    fseek(kitapornek, 0, SEEK_END);
    KITAP *temp= (KITAP*)malloc(sizeof(KITAP));
    KITAP *temp2 = *headptr;
    char kitapID[14] = "0000000000000\0";
    int i=0,j;
    while (kitapbul(headptr,kitapID) != NULL) {
        temp2 = temp2->next;
        kitapID[i]++;
        for (j = 0; j < 13; j++) {
            if (kitapID[j] == 100) {
                kitapID[j] = 48;
                kitapID[j + 1]++;
            }
        }
    }
    strcpy(temp->ISBN,kitapID);
    printf("Kitap Adı: ");
    scanf("%s",temp->kitapAdi);
    printf("Adet: ");
    scanf("%d",&(temp->adet));
    temp->next = *headptr;
    *headptr = temp;
    for(i=0;i<temp->adet;i++){
        KORNEK *kor = (KORNEK*)malloc(sizeof(KORNEK));
        char EtiketNo[20];
        strcpy(EtiketNo,kitapID);
        EtiketNo[13] = '_';
        EtiketNo[14] = i+48;
        EtiketNo[15] = '\0';
        strcpy(kor->EtiketNo,EtiketNo);
        strcpy(kor->Durum,"Uygun");
        kor->next = temp->head;
        temp->head = kor;
        fprintf(kitapornek,"%s,%s\n",kor->EtiketNo,kor->Durum);
    }
    fprintf(kitapornek,"\n");
    fprintf(kitap,"%s,%s,%d\n",temp->kitapAdi,temp->ISBN,temp->adet);
    fclose(kitap);
    fclose(kitapornek);
}
void kitapgoruntule(KITAP** headptr){
    char ISBN[13];
    int i;
    printf("ISBN: ");
    scanf("%s",ISBN);
    KITAP* temp = kitapbul(headptr,ISBN);
    KORNEK* temp2;
    if(temp != NULL){
        printf("Kitap adi = %s\nKitap kodu = %s\nBulunan Adet = %d\n", temp->kitapAdi, temp->ISBN, temp->adet);
        temp2 = temp->head;
        for(i=0;i<temp->adet;i++){
            printf("%d. Kopya = \n",i+1);

            printf("Etiket No = %s,Durum = %s\n",temp2->EtiketNo,temp2->Durum);
            temp2 = temp2->next;
        }
    }
    else{
        printf("Kitap Bulunamadi\n");
    }
}
void kitaplardosyayayaz(KITAP* headptr){
    FILE *kitap = fopen(KITAPLARCSV, "w");
    FILE *kornek = fopen(KITAPORNEKCSV, "w");
    fseek(kitap,0,SEEK_SET);
    KITAP *temp;
    temp = headptr;
    KORNEK *temp2;
    int i;
    while (temp != NULL) {
        fprintf(kitap,"%s,%s,%d\n",temp->kitapAdi,temp->ISBN,temp->adet);
        temp2 = temp->head;
        for(i=0;i<temp->adet;i++){

            fprintf(kornek,"%s,%s\n",temp2->EtiketNo,temp2->Durum);
            temp2 = temp2->next;
        }
        temp = temp->next;
    }
    fclose(kitap);
    fclose(kornek);
}
void kitapsil(KITAP** headptr){
    char kitapISBN[14];
    printf("KITAP ID: ");
    scanf("%s",kitapISBN);
    KITAP *temp;
    temp = *headptr;
    int kitapbulundu=0;
    if(strcmp(temp->ISBN,kitapISBN) == 0){
        *headptr = temp->next;
        free(temp);
        kitaplardosyayayaz(*headptr);
        kitapbulundu = 1;
    }
    else{
        while (temp->next != NULL) {
            if(strcmp(temp->next->ISBN,kitapISBN) == 0){
                temp->next = temp->next->next;
                free(temp->next);
                kitaplardosyayayaz(*headptr);
                kitapbulundu = 1;
                break;
            }
            temp = temp->next;
        }
    }
    if(kitapbulundu== 0){
        printf("Kitap Bulunamadi\n");
    }
}
void kitaplaryazdir(KITAP* headptr) {
    KITAP *temp;
    temp = headptr;
    KORNEK *temp2;
    int i;
    while (temp != NULL) {
        printf("%s,%s,%d\n", temp->kitapAdi, temp->ISBN, temp->adet);
        temp2 = temp->head;
        for(i=0;i<temp->adet;i++){
            printf("%s,%s\n",temp2->EtiketNo,temp2->Durum);
            temp2 = temp2->next;
        }
        temp = temp->next;
    }
}
void kitapGuncelle(KITAP** headptr){
    int i;
    char kitapISBN[13];
    printf("KitapID ISBN: ");
    scanf("%s",kitapISBN);
    KITAP *temp = kitapbul(headptr,kitapISBN);
    if(temp != NULL){
        printf("Kitap adi: ");
        scanf("%s",temp->kitapAdi);
        printf("Adet: ");
        scanf("%d",&(temp->adet));
        for(i=0;i<temp->adet;i++){
            KORNEK *kor = (KORNEK*)malloc(sizeof(KORNEK));
            char EtiketNo[20];
            strcpy(EtiketNo,temp->ISBN);
            EtiketNo[13] = '_';
            EtiketNo[14] = i+48;
            EtiketNo[15] = '\0';
            strcpy(kor->EtiketNo,EtiketNo);
            strcpy(kor->Durum,"Uygun");
            kor->next = temp->head;
            temp->head = kor;
        }
        kitaplardosyayayaz(*headptr);
    }
    else{
        printf("Kitap bulunamadi\n");
    }
}

int saveogrencileroku(OGRENCI** headptr){
    FILE *ogrenci = fopen(OGRENCILERCSV, "r+");
    if(fscanf(ogrenci,"%d") == -1){
        return -1;
    }
    fseek(ogrenci, 0, SEEK_SET);
    while(!feof(ogrenci)){
        OGRENCI *temp;
        temp = (OGRENCI*)malloc(sizeof(OGRENCI));
        fscanf(ogrenci,"%[^,],%[^,],%[^,],%d\n",temp->ad,temp->soyad,temp->ogrID,&(temp->puan));
        temp->next = *headptr;
        *headptr = temp;
    }
    fclose(ogrenci);
}
void ogrencileryazdir(OGRENCI* headptr) {
    OGRENCI *temp;
    temp = headptr;
    while (temp != NULL) {
        printf("Ogrenci Adi = %s\nOgrenci Soyadi=%s\nOgrenci Id = %s\nOgrenci Puan = %d\n", temp->ad, temp->soyad, temp->ogrID, temp->puan);
        temp = temp->next;
    }
}
OGRENCI* ogrencibul(OGRENCI** headptr, char ogrID[9]){
    OGRENCI *temp;
    temp = *headptr;
    while (temp != NULL) {
        if(strcmp(temp->ogrID,ogrID) == 0){
            return temp;
        }
        temp = temp->next;
    }
    return NULL;
}
void ogrenciekle(OGRENCI** headptr){
    int i=0,j;
    FILE *ogrenci = fopen(OGRENCILERCSV, "r+");
    fseek(ogrenci, 0, SEEK_END);
    OGRENCI *temp = (OGRENCI*)malloc(sizeof(OGRENCI));
    OGRENCI *temp2 = *headptr;
    OGRENCI *temp3 = *headptr;
    char ogrID[9] = {48,48,48,48,48,48,48,48,0};
    while(ogrencibul(headptr,ogrID) != NULL){
        temp2 = temp2->next;
        ogrID[i]++;

        for(j=0;j<8;j++){
            if(ogrID[j] == 90){
                ogrID[j] = 48;
                ogrID[j+1]++;
            }
        }
    }
    strcpy(temp->ogrID,ogrID);
    temp->puan = 100;
    printf("Ad: ");
    scanf("%s",temp->ad);
    printf("Soyad: ");
    scanf("%s",temp->soyad);
    temp->next = *headptr;
    temp3->prev = temp;
    *headptr = temp;

    fprintf(ogrenci,"%s,%s,%s,%d\n",temp->ad,temp->soyad,temp->ogrID,temp->puan);
    fclose(ogrenci);
}
void ogrencilerdosyayayaz(OGRENCI* headptr){
    FILE *ogrenci = fopen(OGRENCILERCSV, "w");
    fseek(ogrenci,0,SEEK_SET);
    OGRENCI *temp;
    temp = headptr;
    while (temp->next != NULL) {
        fprintf(ogrenci,"%s,%s,%s,%d\n", temp->ad, temp->soyad, temp->ogrID, temp->puan);
        temp = temp->next;
    }
    fclose(ogrenci);
}
void ogrencisil(OGRENCI** headptr){
    char ogrID[8];
    printf("Ogrenci ID: ");
    scanf("%s",ogrID);
    OGRENCI *temp = ogrencibul(headptr,ogrID);
    if(temp != NULL){
        if(temp->next != NULL){
            temp->next->prev = temp->prev;
        }
        if(temp->prev != NULL){
            temp->prev->next = temp->next;
        }
        else{
            *headptr = temp->next;
        }
        free(temp);
        ogrencilerdosyayayaz(*headptr);
    }
    else{
        printf("Ogrenci Bulunamadi\n");
    }
}
void ogrenciGuncelle(OGRENCI** headptr){
    char ogrID[8];
    printf("Ogrenci ID: ");
    scanf("%s",ogrID);
    OGRENCI *temp = ogrencibul(headptr,ogrID);
    if(temp != NULL){
        printf("Ad: ");
        scanf("%s",temp->ad);
        printf("Soyad: ");
        scanf("%s",temp->soyad);
        printf("Ogrenci ID: ");
        scanf("%s",temp->ogrID);
        printf("Puan: ");
        scanf("%d",&temp->puan);
        ogrencilerdosyayayaz(*headptr);
    }
    else{
        printf("Ogrenci Bulunamadi\n");
    }
}
void cezaliogrencilistele(OGRENCI** headptr){
    OGRENCI *temp;
    temp = *headptr;
    while (temp != NULL) {
        if(temp->puan < 0){
            printf("Ogrenci Adi = %s\nOgrenci Soyadi = %s\nOgrenci Id = %s\nOgrenci Puani = %d\n", temp->ad, temp->soyad, temp->ogrID, temp->puan);
        }
        temp = temp->next;
    }
}

YAZAR * yazarbul(YAZAR** headptr, int yazarID){
    YAZAR *temp;
    temp = *headptr;
    while (temp != NULL) {
        if(yazarID == temp->yazarID){
            return temp;
        }
        temp = temp->next;
    }
    return NULL;
}
void yazarekle(YAZAR** headptr){
    FILE *yazar = fopen(YAZARLARCSV, "r+");
    fseek(yazar, 0, SEEK_END);
    YAZAR *temp = (YAZAR*)malloc(sizeof(YAZAR));
    int yazarID = 0,j;
    YAZAR *temp2 = *headptr;
    while (yazarbul(headptr,yazarID) != NULL) {
        temp2 = temp2->next;
        yazarID++;
    }
    temp->yazarID = yazarID;
    printf("Ad: ");
    scanf("%s",temp->yazarAd);
    printf("Soyad: ");
    scanf("%s",temp->yazarSoyad);

    temp->next = *headptr;
    *headptr = temp;

    fprintf(yazar,"%s,%s,%d\n",temp->yazarAd,temp->yazarSoyad,temp->yazarID);
    fclose(yazar);
}
int saveyazarlaroku(YAZAR** headptr){
    FILE *yazar = fopen(YAZARLARCSV, "r+");
    if(fscanf(yazar,"%d") == -1){
        return -1;
    }
    fseek(yazar, 0, SEEK_SET);
    while(!feof(yazar)){
        YAZAR *temp;
        temp = (YAZAR*)malloc(sizeof(YAZAR));
        fscanf(yazar,"%[^,],%[^,],%d\n",temp->yazarAd,temp->yazarSoyad,&(temp->yazarID));
        temp->next = *headptr;
        *headptr = temp;
    }
    fclose(yazar);
}
void yazarlaryazdir(YAZAR* headptr) {
    YAZAR *temp;
    temp = headptr;
    while (temp != NULL) {
        printf("Yazar Adi = %s\nYazar Soyadi = %sYazar Id = %d\n", temp->yazarAd, temp->yazarSoyad, temp->yazarID);
        temp = temp->next;
    }
}
void yazarlardosyayayaz(YAZAR* headptr){
    FILE *yazar = fopen(YAZARLARCSV, "w");
    fseek(yazar,0,SEEK_SET);
    YAZAR *temp;
    temp = headptr;
    while (temp != NULL) {
        fprintf(yazar,"%s,%s,%d\n",temp->yazarAd,temp->yazarSoyad,temp->yazarID);
        temp = temp->next;
    }
    fclose(yazar);
}
void yazarGuncelle(YAZAR** headptr){
    int yazarID;
    printf("Yazar ID: ");
    scanf("%d",&yazarID);
    YAZAR *temp = yazarbul(headptr,yazarID);
    if(temp != NULL){
        printf("Yazar adi: ");
        scanf("%s",temp->yazarAd);
        printf("Yazar Soyad: ");
        scanf("%s",temp->yazarSoyad);
        yazarlardosyayayaz(*headptr);
    }
    else{
        printf("Yazar bulunamadi\n");
    }
}

int kitapyazareslestir(YAZAR** yheadptr, KITAP** kheadptr, KYAZAR arr[]){
    FILE *kitapyazar = fopen(KITAPYAZARCSV, "r+");
    fseek(kitapyazar, 0, SEEK_END);
    KYAZAR *temp= (KYAZAR*)malloc(sizeof(KYAZAR));
    int yazarNo,i=0;
    char kitapISBN[13];
    printf("Yazar no: ");
    scanf("%d",&yazarNo);

    if(yazarbul(yheadptr,yazarNo)!=NULL){
        temp->YazarID = yazarNo;
    }
    else{
        printf("Yazar bulunamadi\n");
        return -1;
    }
    printf("Kitap ISBN: ");
    scanf("%s",kitapISBN);
    if(kitapbul(kheadptr,kitapISBN)!=NULL){
        strcpy(temp->ISBN,kitapISBN);
    }
    else{
        printf("Kitap bulunamadi\n");
        return -1;
    }
    fprintf(kitapyazar,"%s,%d\n",temp->ISBN,temp->YazarID);
    fclose(kitapyazar);
    while(arr[i].YazarID != -1){
        i++;
    }
    arr[i].YazarID = temp->YazarID;
    strcpy(arr[i].ISBN,temp->ISBN);
}
int saveeslesmeleroku(KYAZAR arr[]){
    FILE* kitapyazar = fopen(KITAPYAZARCSV, "r+");
    int dummy;
    if(fscanf(kitapyazar,"%d",&dummy) == -1){
        return -1;
    }
    fseek(kitapyazar, 0, SEEK_SET);
    int i=0;
    while(!feof(kitapyazar)){
        fscanf(kitapyazar,"%[^,],%d\n",arr[i].ISBN,&(arr[i].YazarID));
        arr[i+1].YazarID = -1;
        i++;
    }
}
void kyazarlaryazdir(KYAZAR arr[]){
    FILE* kitapyazar = fopen(KITAPYAZARCSV, "w");
    int i=0;
    while(arr[i].YazarID != -1){
        fprintf(kitapyazar,"%s,%d\n",arr[i].ISBN,arr[i].YazarID);
        i++;
    }
    fclose(kitapyazar);
}
int kyazarguncelle(YAZAR** yheadptr, KITAP** kheadptr,KYAZAR arr[]){
    int yazarNo;
    char kitapISBN[13];
    printf("Yazar no: ");
    scanf("%d",&yazarNo);
    printf("Kitap ISBN: ");
    scanf("%s",kitapISBN);
    int i=0;
    while(arr[i].YazarID != -1){
        if(arr[i].YazarID == yazarNo && strcmp(arr[i].ISBN,kitapISBN) == 0){
            printf("Yazar no: ");
            scanf("%d",&yazarNo);
            if(yazarbul(yheadptr,yazarNo)!=NULL){
                arr[i].YazarID = yazarNo;
            }
            else{
                printf("Yazar bulunamadi\n");
                return -1;
            }
            printf("Kitap ISBN: ");
            scanf("%s",kitapISBN);
            if(kitapbul(kheadptr,kitapISBN)!=NULL){
                strcpy(arr[i].ISBN,kitapISBN);
            }
            else{
                printf("Kitap bulunamadi\n");
                return -1;
            }
            kyazarlaryazdir(arr);
            return 1;
        }
        else{
            printf("Eslesme bulunamadi\n");
        }
        i++;
    }
    return -1;
}

void yazarsil(YAZAR** headptr, KYAZAR arr[]){
    int yazarID;
    printf("yazarID: ");
    scanf("%d",&yazarID);
    YAZAR *temp;
    temp = *headptr;
    int yazarbulundu=0;
    if(yazarID == temp->yazarID){
        *headptr = temp->next;
        free(temp);
        yazarlardosyayayaz(*headptr);
        yazarbulundu = 1;
    }
    else{
        while (temp->next != NULL) {
            if(temp->next->yazarID == yazarID){
                temp->next = temp->next->next;
                free(temp->next);
                yazarlardosyayayaz(*headptr);
                yazarbulundu = 1;
                break;
            }
            temp = temp->next;
        }
    }
    if(yazarbulundu== 0){
        printf("Kitap Bulunamadi\n");
    }
    else{
        int i=0;
        while(arr[i].YazarID != -1){
            if(arr[i].YazarID == yazarID){
                arr[i].YazarID = -1;
                strcpy(arr[i].ISBN,"");
            }
            i++;
        }
        kyazarlaryazdir(arr);
    }
}



KODUNCLISTE* depodabul(KODUNCLISTE** headptr, char ogrNo[8], char kitapISBN[20]){
    KODUNCLISTE *temp = *headptr;
    KODUNCLISTE *depotemp = NULL;
    int sifirbulundu=0;
    while(temp != NULL){
        if(strcmp(temp->odunc->EtiketNo,kitapISBN) == 0 &&  strcmp(temp->odunc->ogrID,ogrNo)==0){
            sifirbulundu = !sifirbulundu;
            depotemp = temp;
        }
        temp = temp->next;
    }
    if(sifirbulundu == 1){
        return depotemp;
    }
    else{
        return NULL;
    }
}
KODUNCLISTE* depodabulkitapla(KODUNCLISTE** headptr, char kitapISBN[20]){
    KODUNCLISTE *temp = *headptr;
    KODUNCLISTE *depotemp = NULL;
    int sifirbulundu=0;
    while(temp != NULL){
        if(strcmp(temp->odunc->EtiketNo,kitapISBN) == 0){
            sifirbulundu = !sifirbulundu;
            depotemp = temp;
        }

        temp = temp->next;
    }
    if(sifirbulundu == 1){
        return depotemp;
    }
    else{
        return NULL;
    }
}
KODUNCLISTE* depodabulogrencino(KODUNCLISTE** headptr, char ogrNo[20]){
    KODUNCLISTE *temp = *headptr;
    KODUNCLISTE *depotemp = NULL;
    int sifirbulundu=0;
    while(temp != NULL){
        if(strcmp(temp->odunc->ogrID,ogrNo) == 0){
            sifirbulundu++;
            depotemp = temp;
        }
        if(strcmp(temp->odunc->ogrID,ogrNo) == 0 && temp->odunc->islemTipi == 1){
            sifirbulundu--;
        }

        temp = temp->next;
    }
    if(sifirbulundu > 0){
        return depotemp;
    }
    else{
        return NULL;
    }
}
int oduncal(OGRENCI *ohead,KITAP *khead,KODUNCLISTE** kodhead){
    FILE *oduncfile = fopen(KITAPODUNCCSV,"r+");
    fseek(oduncfile,0,SEEK_END);
    int i;
    char ogrNo[10];
    printf("Ogrenci no: ");
    scanf("%s",ogrNo);
    OGRENCI *otemp = ogrencibul(&ohead,ogrNo);
    if(otemp->puan<0){
        printf("Ogrenci puani eksik\n");
        return -1;
    }
    char kitapISBN[13];
    printf("Kitap ISBN: ");
    scanf("%s",kitapISBN);
    KITAP *ktemp = kitapbul(&khead,kitapISBN);
    KORNEK *ktemp2;
    if(otemp != NULL && ktemp != NULL){
        KODUNCLISTE *temp = (KODUNCLISTE*)malloc(sizeof(KODUNCLISTE));
        KODUNC *kodunc = (KODUNC*)malloc(sizeof(KODUNC));
        TARIH *tarih = (TARIH*)malloc(sizeof(TARIH));
        strcpy(kodunc->ogrID,ogrNo);
        kodunc->islemTipi = 0;
        printf("Tarih girmek icin 1,bugunun tarihini almak icin 2: ");
        int secim,gun,ay,yil;
        int kitapfound = 0,bulunankitapsirasi=0;
        scanf("%d",&secim);
        if(secim == 1){
            printf("Gun: ");
            scanf("%d",&gun);
            printf("Ay: ");
            scanf("%d",&ay);
            printf("Yil: ");
            scanf("%d",&yil);
            tarih->gun = gun;
            tarih->ay = ay;
            tarih->yil = yil;
        }
        else{
            time_t t = time(NULL);
            struct tm tm = *localtime(&t);
            tarih->gun = tm.tm_mday;
            tarih->ay = tm.tm_mon + 1;
            tarih->yil = tm.tm_year + 1900;
        }
        kodunc->islemTarihi = *tarih;
        ktemp2 = ktemp->head;

        for(i=0;i<ktemp->adet;i++){
            char EtiketNo[20];
            strcpy(EtiketNo,ktemp->ISBN);
            EtiketNo[13] = '_';
            EtiketNo[14] = i+48;
            EtiketNo[15] = '\0';
            if(depodabulkitapla(kodhead,EtiketNo) == NULL){
                strcpy(kodunc->EtiketNo,EtiketNo);

                kitapfound = 1;
                bulunankitapsirasi = i;
            }

        }
        for (i=0;i<bulunankitapsirasi;i++){
            ktemp2 = ktemp2->next;
        }
        strcpy(ktemp2->Durum,"Odunc");
        if(kitapfound==0){
            printf("Stokta kitaptan kalmamis.\n");
            return -1;
        }
        temp->odunc = kodunc;
        temp->next = *kodhead;
        kitaplardosyayayaz(khead);
        *kodhead = temp;
        fprintf(oduncfile,"%s,%s,%d,%d.%d.%d\n",kodunc->EtiketNo,kodunc->ogrID,kodunc->islemTipi,kodunc->islemTarihi.gun & 0b00011111,kodunc->islemTarihi.ay & 0b0001111,kodunc->islemTarihi.yil);
        fclose(oduncfile);
    }
    else{
        printf("Ogrenci veya kitap bulunamadi\n");
    }
}
int oduncver(OGRENCI *ohead,KITAP *khead,KODUNCLISTE** kodhead){
    FILE *oduncfile = fopen(KITAPODUNCCSV,"r+");
    fseek(oduncfile,0,SEEK_END);
    int i;
    char ogrNo[10];
    printf("Ogrenci no: ");
    scanf("%s",ogrNo);

    OGRENCI *otemp = ogrencibul(&ohead,ogrNo);
    char kitapISBN[13];
    printf("Kitap ISBN: ");
    scanf("%s",kitapISBN);
    KITAP *ktemp = kitapbul(&khead,kitapISBN);
    KORNEK *ktemp2;
    if(otemp != NULL && ktemp != NULL){
        KODUNCLISTE *temp = (KODUNCLISTE*)malloc(sizeof(KODUNCLISTE));
        KODUNC *kodunc = (KODUNC*)malloc(sizeof(KODUNC));
        TARIH *tarih = (TARIH*)malloc(sizeof(TARIH));
        strcpy(kodunc->ogrID,ogrNo);
        kodunc->islemTipi = -1;
        printf("Tarih girmek icin 1,bugunun tarihini almak icin 2: ");
        int secim,gun,ay,yil;
        int kitapfound = 0,bulunankitapsirasi=0;
        scanf("%d",&secim);
        if(secim == 1){
            printf("Gun: ");
            scanf("%d",&gun);
            printf("Ay: ");
            scanf("%d",&ay);
            printf("Yil: ");
            scanf("%d",&yil);
            tarih->gun = gun;
            tarih->ay = ay;
            tarih->yil = yil;
        }
        else{
            time_t t = time(NULL);
            struct tm tm = *localtime(&t);
            tarih->gun = tm.tm_mday;
            tarih->ay = tm.tm_mon + 1;
            tarih->yil = tm.tm_year + 1900;
        }
        kodunc->islemTarihi = *tarih;
        ktemp2 = ktemp->head;

        for(i=0;i<ktemp->adet;i++){
            char EtiketNo[20];
            strcpy(EtiketNo,ktemp->ISBN);
            EtiketNo[13] = '_';
            EtiketNo[14] = i+48;
            EtiketNo[15] = '\0';
            if(depodabulkitapla(kodhead,EtiketNo) != NULL){
                strcpy(kodunc->EtiketNo,EtiketNo);
                kitapfound = 1;
                bulunankitapsirasi = i;
            }

        }
        for (i=0;i<bulunankitapsirasi;i++){
            ktemp2 = ktemp2->next;
        }
        strcpy(ktemp2->Durum,"Uygun");
        if(kitapfound==0){
            printf("Bu kitap alinmamis.\n");
            return -1;
        }
        temp->odunc = kodunc;
        temp->next = *kodhead;
        kitaplardosyayayaz(khead);
        *kodhead = temp;
        fprintf(oduncfile,"%s,%s,%d,%d.%d.%d\n",kodunc->EtiketNo,kodunc->ogrID,kodunc->islemTipi & 0b00000001,kodunc->islemTarihi.gun & 0b00011111,kodunc->islemTarihi.ay & 0b0001111,kodunc->islemTarihi.yil);
        fclose(oduncfile);
    }
    else{
        printf("Ogrenci veya kitap bulunamadi\n");
    }
}
int saveoduncleroku(KODUNCLISTE** headptr){

    FILE *odunckitap = fopen(KITAPODUNCCSV, "r+");
    int dummy;
    int islemTipi,gun,ay,yil;
    char islemTipic[5],gunc[5],ayc[5];
    if(fscanf(odunckitap,"%d",&dummy) == -1){
        return -1;
    }
    fseek(odunckitap, 0, SEEK_SET);
    while(!feof(odunckitap)){
        KODUNCLISTE* ktemp = (KODUNCLISTE*)malloc(sizeof(KODUNCLISTE));
        KODUNC *temp;
        temp = (KODUNC*)malloc(sizeof(KODUNC));
        TARIH *tarih = (TARIH*)malloc(sizeof(TARIH));
        temp->islemTarihi = *tarih;
        fscanf(odunckitap,"%[^,],%[^,],%[^,],%[^.].%[^.].%d\n",temp->EtiketNo,temp->ogrID,islemTipic,gunc,ayc,&yil);
        islemTipi = atoi(islemTipic);
        gun = atoi(gunc);
        ay = atoi(ayc);
        tarih->gun = gun;
        tarih->ay = ay;
        tarih->yil = yil;
        temp->islemTipi = islemTipi;
        temp->islemTarihi = *tarih;
        ktemp->odunc = temp;
        ktemp->next = *headptr;
        *headptr = ktemp;

    }
    fclose(odunckitap);
}

void kitapteslimetmeyenler(KODUNCLISTE** headptr,OGRENCI *ohead){
    OGRENCI *temp;
    temp = ohead;
    while (temp != NULL) {
        if(depodabulogrencino(headptr,temp->ogrID) != NULL){
            printf("Ogrenci Adi = %s\nOgrenci Soyadi = %s\nOgrenci Id = %s\nOgrenci Puani = %d\n", temp->ad, temp->soyad, temp->ogrID, temp->puan);
        }
        temp = temp->next;
    }
}
void raftakiKitaplar(KODUNCLISTE** headptr,KITAP *khead){
    KITAP *temp;
    temp = khead;
    KORNEK *temp2;
    int i;
    while (temp != NULL) {
        temp2 = temp->head;
        for(i=0;i<temp->adet;i++){
            char EtiketNo[20];
            ;
            if(depodabulkitapla(headptr,temp2->EtiketNo) == NULL){
                printf("Kitap Adi = %s\nISBN = %s\nAdet = %d\n", temp->kitapAdi, temp->ISBN, temp->adet);
            }
            temp2 = temp2->next;
        }
        temp = temp->next;
    }
}
void teslimedilmeyenkitaplar(KODUNCLISTE** headptr){
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    int bugun = tm.tm_mday;
    int buay = tm.tm_mon + 1;
    int buyil = tm.tm_year + 1900;
    KODUNCLISTE *temp = *headptr;
    KODUNCLISTE *depotemp = NULL;
    int gecengun;
    int yil,ay,gun;

    while(temp != NULL){
        yil = temp->odunc->islemTarihi.yil;
        ay = temp->odunc->islemTarihi.ay & 0b0001111;
        gun = temp->odunc->islemTarihi.gun & 0b00011111;
        gecengun = (buyil - yil)*365 + (buay - ay)*30 + (bugun - gun);
        if(temp->odunc->islemTipi == 0){
            if(gecengun > 15){
                printf("Getirilmeyen kitabin kodu = %s\nKac gun gecti=%d\n",temp->odunc->EtiketNo,gecengun);
            }
        }
        temp = temp->next;
    }
}
void cezalariver(KODUNCLISTE** headptr,OGRENCI **ohead){
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    int bugun = tm.tm_mday;
    int buay = tm.tm_mon + 1;
    int buyil = tm.tm_year + 1900;
    KODUNCLISTE *odunctempiceri = *headptr;
    KODUNCLISTE *teslimOdunc = NULL;
    KODUNCLISTE *depotemp = *headptr;
    int teslimbulundu = 0;
    int gecengun;
    int yil,ay,gun,yil2,ay2,gun2;
    int ceza = 10;
    OGRENCI *otemp = *ohead;
    while(otemp!=NULL){
        otemp->puan = 100;
        otemp = otemp->next;
    }
    while(depotemp!=NULL){
        if(depotemp->odunc->islemTipi==0){
            odunctempiceri = *headptr;
            while(odunctempiceri!=NULL){
                if(odunctempiceri->odunc->islemTipi==1 && strcmp(depotemp->odunc->ogrID,odunctempiceri->odunc->ogrID)==0 && strcmp(depotemp->odunc->EtiketNo,odunctempiceri->odunc->EtiketNo)==0){
                    teslimbulundu = 1;
                    teslimOdunc = odunctempiceri;
                    break;
                }
                odunctempiceri = odunctempiceri->next;
            }
            if(teslimbulundu == 1){
                yil = depotemp->odunc->islemTarihi.yil;
                ay = depotemp->odunc->islemTarihi.ay & 0b0001111;
                gun = depotemp->odunc->islemTarihi.gun & 0b00011111;
                yil2 = teslimOdunc->odunc->islemTarihi.yil;
                ay2 = teslimOdunc->odunc->islemTarihi.ay & 0b0001111;
                gun2 = teslimOdunc->odunc->islemTarihi.gun & 0b00011111;

                gecengun = (yil2 - yil)*365 + (ay2 - ay)*30 + (gun2 - gun);
                if(gecengun > 15){

                }
            }
            else{
                yil = depotemp->odunc->islemTarihi.yil;
                ay = depotemp->odunc->islemTarihi.ay & 0b0001111;
                gun = depotemp->odunc->islemTarihi.gun & 0b00011111;
                gecengun = (buyil - yil)*365 + (buay - ay)*30 + (bugun - gun);
                if(gecengun > 15){
                    OGRENCI *temp = ogrencibul(ohead,depotemp->odunc->ogrID);
                    temp->puan = temp->puan - ceza;
                }
            }
        }
        depotemp = depotemp->next;
    }


    ogrencilerdosyayayaz(*ohead);
}
void yazarGoruntule(YAZAR *yhead,KITAP *khead, KYAZAR *kyazarlar) {
    YAZAR *temp;
    int yazarId;
    int i=0;
    printf("Yazar ID giriniz: ");
    scanf("%d",&yazarId);
    temp = yazarbul(&yhead,yazarId);
    if(temp == NULL){
        printf("Yazar bulunamadi\n");
    }
    else{
        printf("Yazar Adi = %s\nYazar Soyadi = %s\nYazar Id = %d\n", temp->yazarAd, temp->yazarSoyad, temp->yazarID);
    }
    while(kyazarlar[i].YazarID != -1){
        if(kyazarlar[i].YazarID == yazarId){
            KITAP* kitap = kitapbul(&khead,kyazarlar[i].ISBN);
            printf("Kitap Adi = %s\nKitap ISBN = %s\n", kitap->kitapAdi, kitap->ISBN);
        }
        i++;
    }
}
void ogrencigoruntule(OGRENCI** headptr,KODUNCLISTE** koduncheadptr){
    char ogrID[8];
    printf("Ogrenci ID: ");
    scanf("%s",ogrID);
    OGRENCI *temp = ogrencibul(headptr,ogrID);
    if(temp != NULL){
        printf("Ogrenci Adi = %s\nOgrenci Soyadi = %s\nOgrenci Id = %s\nOgrenci Puani = %d\n", temp->ad, temp->soyad, temp->ogrID, temp->puan);
        KODUNCLISTE *ktemp = *koduncheadptr;
        while(ktemp != NULL){
            if(strcmp(ktemp->odunc->ogrID,ogrID)==0){
                printf("Kitap Etiket Numarasi = %s\nIslem Tipi = %d\nIslem Tarihi= %d.%d.%d\n", ktemp->odunc->EtiketNo, ktemp->odunc->islemTipi & 0b00000001,ktemp->odunc->islemTarihi.gun & 0b00011111,ktemp->odunc->islemTarihi.ay & 0b00001111,ktemp->odunc->islemTarihi.yil);
            }
            ktemp = ktemp->next;
        }
    }
    else{
        printf("Ogrenci Bulunamadi\n");
    }
}

void mainMenu(YAZAR *yazarhead,OGRENCI *ohead,KITAP *khead,KYAZAR kyazarlar[],KODUNCLISTE *kodunchead){
    int secim;
    while(1) {
        printf("1- Ogrenci Islemleri\n2- Kitap Islemleri\n3- Yazar Islemleri\n4- Cikis\n");
        scanf("%d", &secim);
        switch (secim) {
            case 1:
                printf("1- Ogrenci Ekle,Sil,Guncelle\n2- Ogrenci Bilgisi Goruntuleme\n3- Kitap teslim etmeyen ogrencileri listele\n4- Cezali Ogrenci listele\n5- Tum Ogrencileri listele\n6- Kitap Odunc Al/Teslim Et\n7- Ana Menu\n");
                scanf("%d", &secim);
                switch (secim) {
                    case 1:
                        printf("1- Ogrenci Ekle\n2- Ogrenci Sil\n3- Ogrenci Güncelle\n4- Ana Menu\n");
                        scanf("%d", &secim);
                        switch (secim) {
                            case 1:
                                ogrenciekle(&ohead);
                                break;
                            case 2:
                                ogrencisil(&ohead);
                                break;
                            case 3:
                                ogrenciGuncelle(&ohead);
                                break;
                            case 4:
                                break;
                            default:
                                printf("Yanlis secim\n");
                                break;
                        }
                        break;
                    case 2:
                        ogrencigoruntule(&ohead,&kodunchead);
                        break;
                    case 3:
                        kitapteslimetmeyenler(&kodunchead,ohead);
                        break;
                    case 4:
                        cezaliogrencilistele(&ohead);
                        break;
                    case 5:
                        ogrencileryazdir(ohead);
                        break;
                    case 6:
                        printf("1- Kitap Odunc Al\n2- Kitap Teslim Et\n3- Ana Menu\n");
                        scanf("%d", &secim);
                        switch (secim) {
                            case 1:
                                oduncal(ohead, khead, &kodunchead);
                                break;
                            case 2:
                                oduncver(ohead, khead, &kodunchead);
                                break;
                            case 3:
                                break;
                            default:
                                printf("Yanlis secim)\n");
                                break;
                        }
                        break;
                    case 7:
                        break;
                    default:
                        printf("Hatali secim\n");
                        break;


                }
                break;
            case 2:
                printf("1- Kitap Ekle, Sil, Güncelle\n2- Kitap Bilgisi Görüntüleme:\n3- Raftaki Kitapları Listele\n4- Zamanında Teslim Edilmeyen Kitapları Listele\n5- Kitap-Yazar Eşleştir\n6- Kitabın Yazarını Güncelle\n7- Ana Menu\n");
                scanf("%d", &secim);
                switch (secim) {
                    case 1:
                        printf("1- Kitap Ekle\n2- Kitap Sil\n3- Kitap Güncelle\n4- Ana Menu\n");
                        scanf("%d", &secim);
                        switch (secim) {
                            case 1:
                                kitapekle(&khead);
                                break;
                            case 2:
                                kitapsil(&khead);
                                break;
                            case 3:
                                kitapGuncelle(&khead);
                                break;
                            case 4:
                                break;
                            default:
                                printf("Yanlis secim\n");
                                break;
                        }
                        break;
                    case 2:
                        kitapgoruntule(&khead);
                        break;
                    case 3:
                        raftakiKitaplar(&kodunchead,khead);
                        break;
                    case 4:
                        teslimedilmeyenkitaplar(&kodunchead);
                        break;
                    case 5:
                        kitapyazareslestir(&yazarhead,&khead,kyazarlar);
                        break;
                    case 6:
                        kyazarguncelle(&yazarhead,&khead,kyazarlar);
                        break;
                    case 7:
                        break;
                    default:
                        printf("Yanlis secim\n");
                        break;
                }
                break;
            case 3:
                printf("1- Yazar Ekle, Sil, Güncelle\n2- Yazar Bilgisi Görüntüleme\n3- Ana Menu\n");
                scanf("%d", &secim);
                switch (secim) {
                    case 1:
                        printf("1- Yazar Ekle\n2- Yazar Sil\n3- Yazar Güncelle\n4- Ana Menu\n");
                        scanf("%d", &secim);
                        switch (secim) {
                            case 1:
                                yazarekle(&yazarhead);
                                break;
                            case 2:
                                yazarsil(&yazarhead,kyazarlar);
                                break;
                            case 3:
                                yazarGuncelle(&yazarhead);
                                break;
                            case 4:
                                break;
                            default:
                                printf("Yanlis secim\n");
                                break;
                        }
                        break;
                    case 2:
                        yazarGoruntule(yazarhead,khead,kyazarlar);
                        break;
                    case 3:
                        break;
                    default:
                        printf("Yanlis secim\n");
                        break;
                }
                break;
            case 4:
                exit(0);
            default:
                printf("Hatali secim\n");
                break;
        }
    }
}

int main() {
    YAZAR *yazarhead = NULL;
    OGRENCI *ohead = (OGRENCI*)malloc(sizeof(OGRENCI));
    strcpy(ohead->ad, "Created for prev");
    strcpy(ohead->soyad, "Not a student");
    KITAP *khead = NULL;
    KYAZAR kyazarlar[1000];
    kyazarlar[0].YazarID = -1;
    KODUNCLISTE *kodunchead = NULL;

    saveoduncleroku(&kodunchead);
    saveeslesmeleroku(kyazarlar);
    savekitaplaroku(&khead);
    saveogrencileroku(&ohead);
    saveyazarlaroku(&yazarhead);
    cezalariver(&kodunchead, &ohead);

    mainMenu(yazarhead,ohead,khead,kyazarlar,kodunchead);


    return 0;
}