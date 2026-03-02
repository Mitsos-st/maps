#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//Struct για την αποθήκευση των δεδομένων του αρχείου
typedef struct data{
    int distance;
    char *city1;
    char *city2;
} * Data;

//Struct για την αποθήκευση των πόλεων ως αριθμούς
typedef struct number_city{
    int city1_number;
    int city2_number;
    int distance;
} * City;

//Struct για την σύνδεση των αριθμών με τα ονόματα των πόλεων	
typedef struct city_connection{
    int city_number;
    char *city_name;
} * City_connection;

//Συνάρτηση για τον υπολογισμό της τετραγωνικής ρίζας
//Εφόσον δεν επιτρέπεται η χρήση της math.h
int my_sqrt(int n){
    //Υπολογίζει μόνο τα τέλεια τετράγωνα
    for(int i=0;i<n;i++){
        if(i*i==n){
            return i;
        }
    }
    //Επιστρέφει -1 σε περίπτωση που δεν είναι τέλειο τετράγωνο
    return -1;
}

//Συνάρτηση για την ανταλλαγή τιμών
void swap(int *a,int *b){
    int temp=*a;
    *a=*b;
    *b=temp;
}

//Συνάρτηση για την αντιστοίχιση των πόλεων σε αριθμούς
//Παίρνει ως όρισμα τα δεδομένα του αρχείου και το μέγεθος του
City number_city(Data root,int size){
    //Δέσμευση μνήμης για το struct City
    City city=malloc(size*sizeof(struct number_city));
    //Έλεγχος για την επιτυχία της δέσμευσης μνήμης
    if(!city){
        fprintf(stderr,"Failed to allocate memory\n");
        exit(1); //Έξοδος από το πρόγραμμα με τη χρήση της exit
    }
    //Αρχικοποίηση του count στο 3 διότι οι πρώτες δύο πόλεις αντιστοιχούν στους αριθμούς 1 και 2
    int count=3;
    int flag=0;
    //Διατρέχει τον πίνακα από struct με τα δεδομένα του αρχείου
    for(int i=0;i<size;i++){
        if(i==0){
            //Αντιστοιχεί τις πρώτες δύο πόλεις στους αριθμούς 1 και 2
            city[i].city1_number=1;
            city[i].city2_number=2;
            city[i].distance=root[i].distance;
        }else{
            int j=0;
            flag=0;
            //Ελέγχει αν η πόλη υπάρχει ήδη στο struct με τους αριθμούς
            while(j<i){
                //Αν υπάρχει, αντιστοιχεί τον αριθμό της πόλης
                //Και αλλάζει την flag σε 1
                if(!strcmp(root[i].city1,root[j].city1)){
                    city[i].city1_number=city[j].city1_number;
                    flag=1;
                    break;
                }
                if(!strcmp(root[i].city1,root[j].city2)){
                    city[i].city1_number=city[j].city2_number;
                    flag=1;
                    break;
                }
                j++;
            }
            //Αν η πόλη δεν υπάρχει (flag=0), την αντιστοιχεί σε νέο αριθμό
            if(flag==0){
                city[i].city1_number=count;
                count++;
            }
            flag=0;
            j=0;
            //Ομοίως για τις δεξία πόλεις
            while(j<i){
                if(!strcmp(root[i].city2,root[j].city1)){
                    city[i].city2_number=city[j].city1_number;
                    flag=1;
                    break;
                }
                if(!strcmp(root[i].city2,root[j].city2)){
                    city[i].city2_number=city[j].city2_number;
                    flag=1;
                    break;
                }
                j++;
            }
            if(flag==0){
                city[i].city2_number=count;
                count++;
            }
            //Τέλος, αντιστοιχεί την απόσταση των πόλεων
            city[i].distance=root[i].distance;
        }
    }
    //Επιστροφή του struct με τους αριθμούς
    return city;
}

//Συνάρτηση για την ταξινόμηση των στηλών του struct City
void sort_city_column(City city,int size){
    //Διατρέχει το struct City
    for(int i=0;i<size-1;i++){
        for(int j=0;j<size-i-1;j++){
            //Αν ο αριθμός της πόλης που διατρέχουμε είναι μεγαλύτερος από τον αριθμό της επόμενης
            if(city[j].city1_number>city[j+1].city1_number){
                //Τότε ανταλλάσσει τους αριθμούς των πόλεων
                //και την απόσταση τους
                swap(&city[j].city1_number,&city[j+1].city1_number);
                swap(&city[j].city2_number,&city[j+1].city2_number);
                swap(&city[j].distance,&city[j+1].distance);
            }
        }
    }
    //Ομοίως ταξινόμιση για τις δεξιές πόλεις
    //όμως με τον περιορισμό ότι οι αριστερά πόλεις πρέπει να είναι ίδιες
    for(int i=0;i<size-1;i++){
        for(int j=0;j<size-i-1;j++){
            if(city[j].city2_number>city[j+1].city2_number && city[j].city1_number==city[j+1].city1_number){
                swap(&city[j].city1_number,&city[j+1].city1_number);
                swap(&city[j].city2_number,&city[j+1].city2_number);
                swap(&city[j].distance,&city[j+1].distance);
            }
        }
    }
}

//Συνάρτηση για την ταξινόμηση των γραμμών του struct City
void sort_city_raw(City city,int size){
    //Διατρέχει το struct City
    for(int i=0;i<size;i++){
        //Αν ο αριθμός της αριστερά πόλης είναι μεγαλύτερος από τον αριθμό της δεξιάς
        if(city[i].city1_number>city[i].city2_number){
            //Τότε ανταλλάσσει τους αριθμούς των πόλεων
            swap(&city[i].city1_number,&city[i].city2_number);
        }
    }
}

//Συνάρτηση για την εύρεση των ονομάτων των πόλεων
//με τη χρήση του struct City_connection το οποίο αντιστοιχεί τους αριθμούς με τα ονόματα
char **find_city_names(City_connection index_name,int n,int *array,int max){
    //Πέρνει ως ορίσματα και τον πίνακα με τους αριθμούς των πόλεων της βέλτιστης διαδρομής
    //Το μέγιστο μήκος των ονομάτων των πόλεων (int max)
    //Το πλήθος των πόλεων (int n)
   
    char **city_names=malloc(n*sizeof(char*)); //Δέσμευση μνήμης για τον πίνακα με τα ονόματα των πόλεων
    if(!city_names){
        fprintf(stderr,"Failed to allocate memory\n");
        exit(1);
    }
    for(int i=0;i<n;i++){
        city_names[i]=malloc(max*sizeof(char));
        if(!city_names[i]){
            fprintf(stderr,"Failed to allocate memory\n");
            exit(1);
        }
    }
    //Συγκρίνει τους δύο πίνακες
    for(int i=0;i<n;i++){
        for(int j=0;j<n;j++){
            //Αν έχουν την ίδια τιμή
            //Αντιγράφει το όνομα της πόλης του struct city_connection 
            //στον πίνακα των ονομάτων των πόλεων της βέλτιστης διαδρομής 
            if(index_name[j].city_number==array[i]){
                strcpy(city_names[i],index_name[j].city_name);
            }
        }
    }
    //Επιστρέφει τον πίνακα των ονομάτων
    return city_names;
}

//Συνάρτηση όπου αρχικοποιεί το struct city_connection
//Το οποίο συνδέει τα ονόματα των πόλεων με τους αριθμούς
City_connection inisialize_index_name(Data root,int size,int n,City city,int max){
    //Πέρνει ως ορίσματα το struct με τα στοιχεία του αρχείου (Data root)
    //Το μέγεθως του αρχείου (το πλήθος των γραμμών) (int size)
    //Το πλήθος των πόλεων (int n)
    //Το struct με τις πόλεις ως αριθμούς (City city)
    //Το μέγιστο μήκος των ονομάτων των πόλεων (int max)

    //Δέσμευση μνή,ης για το struct city_connection
    City_connection index_name=malloc(n*sizeof(struct city_connection));
    if(!index_name){
        fprintf(stderr,"Failed to allocate memory\n");
        exit(1);
    }
    //Δέσμευση μνήμης για τα ονόματα των πόλεων
    for(int k=0;k<n;k++){
        index_name[k].city_name=malloc(max*sizeof(char));
        if(!index_name[k].city_name){
            fprintf(stderr,"Failed to allocate memory\n");
            exit(1);
        }
    }
    //Διατρέχει κάθε στοιχείο του city και του root
    //Και αποθηκεύει το νούμερο και το όνομα
    for(int i=0;i<n;i++){
        for(int j=0;j<size;j++){
            //Αν το νούμερο της αριστερής πόλης είναι ίσο με το i+1
            //Δηλαδή το index του city_connection
            //Αποθηκεύει τα στοιχεία σε αυτό το index
            if(city[j].city1_number==i+1){ //Ελέγχει το i+1 δίοτι η αρίθμηση των πόλεων ξεκίναει από το 1
                index_name[i].city_number=city[j].city1_number;
                strcpy(index_name[i].city_name,root[j].city1);
            }
            //Ομοίως για τη δεξιά πόλη
            if(city[j].city2_number==i+1){
                index_name[i].city_number=city[j].city2_number;
                strcpy(index_name[i].city_name,root[j].city2);
            }
        }
    }
    //Επιστρέφει το city_connection
    return index_name;
}

//Συνάρτηση που υλοποιεί τον αλγόριθμο Held-Karp
int Held_karp_function(City city,int size,City_connection index_name,int n,int max){
    //Παίρνει ως ορίσματα το struct με τις πόλεις ως αριθμούς (City city)
    //Το μέγεθος του αρχείου (int size)
    //Το struct που συνδέει τα ονόματα των πόλεων με τους αριθμούς (City_connection index_name)
    //Το πλήθος των πόλεων (int n)
    //Το μέγιστο μήκος των ονομάτων των πόλεων (int max)

    //Πίνακας που αποθηκεύει κωδικοποιημένα τις αποστάσεις
    //Για μείωση του χρόνου της συνάρτησης
    int distance[n*100+n];
    for(int i=0;i<size;i++){
        distance[city[i].city1_number*100+city[i].city2_number]=city[i].distance;
        distance[city[i].city2_number*100+city[i].city1_number]=city[i].distance;
    }
    //Δέσμευση μνήμης για τους πίνακες g και pred
    //Ο g αποθηκεύει τις βέλτιστες διαδρομές
    //Ο pred αποθηκεύει τη σειρά των πόλεων
    int** g = malloc((1<<n) * sizeof(int*));
    int** pred = malloc((1<<n) * sizeof(int*));
    if(!g || !pred){
        fprintf(stderr,"Failed to allocate memory\n");
        exit(1);
    }
    for(int i=0;i<1<<n;i++){
        g[i]=malloc(n * sizeof(int));
        pred[i]=malloc(n * sizeof(int));
        if(!g[i] || !pred[i]){
            fprintf(stderr,"Failed to allocate memory\n");
            exit(1);
        }
    }
    //Αρχικοποίηση των g[{κ}][κ]
    for(int k=2;k<=n;k++){
        g[1<<(k-1)][k-1]=distance[1*100+k];
    }
    //Υπολογισμός των αποστάσεων για όλα τα υποσύνολα πόλεων
    for(int s=2;s<=n-1;s++){
        //Το Α συμβολίζει ένα σύνολο αριθμών(το σύνολο των πόλεων) κωδικοποιημένο σε έναν αριθμό
        for(int A=2;A<1<<n;A++){ 
            if(__builtin_popcount(A)==s){ //Χρήση __builtin_popcount για την μέτρηση των bit του Α που είναι 1
                for(int k=2;k<=n;k++){
                    if((A & (1<<(k-1)))!=0){
                        int min=1000000;
                        for(int i=2;i<=n;i++){
                            if((A & (1<<(i-1)))!=0 && i!=k){
                                int value=g[A-(1<<(k-1))][i-1]+distance[i*100+k];
                                if(value<min){
                                    min=value;
                                    pred[A][k-1]=i;
                                }
                            }
                        }
                        g[A][k-1]=min;
                    }
                }
            }
        }
    }
    //Εύρεση της βέλτιστης διαδρομής
    int temp=0;
    int temp2=0;
    int min=1000000;
    //Εύρεση της βέλτιστης διαδρομής και της τελευταίας πολής
    for(int j=2;j<=n;j++){
        int value=g[(1<<n)-2][j-1];
        if(value<min){
            min=value;
            temp=j;
        }
    }
    //Γυρίζοντας προς τα πίσω απο την τελευταία
    //Βρίσκει όλη τη σειρά των πόλεων
    int minus=0;
    int array[n]; //Πίνακας για την αποθήκευση της σειράς των πόλεων
    array[0]=temp; //Η πρώτη τιμή είναι η τελευταία πόλη
    //Αφαιρώντας μία μία τις πόλεις απο το σύνολο των πόλεων βρίσκει τη σείρα
    for(int k=0;k<n-2;k++){
        temp2=pred[(1<<n)-2-minus][temp-1];
        array[k+1]=temp2;
        minus+=(1<<(temp-1));
        temp=temp2;
    }
    array[n-1]=1; //Η τελευταία τιμή είναι η πρώτη πόλη
    //Εύρεση των ονομάτων των πόλεων της βέλτιστης διαδρομής
    char **city_names=find_city_names(index_name,n,array,max);
    //Τύπωση της βέλτιστης διαδρομής
    for(int i=n-1;i>0;i--){
        printf("%s -(%d)-> ",city_names[i],distance[array[i]*100+array[i-1]]);
    }
    printf("%s\n",city_names[0]);
    //Απελευθέρωση της δεσμευμένης μνήμης
    for(int i=0;i<1<<n;i++){
        free(pred[i]);
    }
    free(pred);
    for(int i=0;i<1<<n;i++){
        free(g[i]);
    }
    free(g);
    for(int i=0;i<n;i++){
        free(city_names[i]);
    }
    free(city_names);
    //Επιστροφή της ελάχιστης απόστασης
    return min;
}

//Συνάρτηση main
int main(int argc,char *argv[]){
    //Αν τα ορίσματα δεν είναι δύο τυπώνεται μήνυμα λάθους
    if(argc!=2){
        fprintf(stderr,"Usage: %s <filename>\n",argv[0]);
        return 1;
    }
    //Ανοίγει το αρχείο
    FILE *fileToRead;
    fileToRead = fopen(argv[1], "r");
    if (!fileToRead) {
        fprintf(stderr, "Error: Cannot open file %s\n", argv[1]);
        return 1;
    }
    //Μετακίνησει του δείκτη στο τέλος
    fseek(fileToRead, 0, SEEK_END);
    int size = ftell(fileToRead);
    //Ελέγχει αν το αρχείο είναι κενό και τυπώνει αντίστοιχο μήνυμα λάθους
    if(size == 0){
        fprintf(stderr,"Error: File %s is empty\n",argv[1]);
        return 1;
    }
    //Επιστροφή του δείκτη στην αρχή
    rewind(fileToRead);
    char c;
    int j=0,count=0,max=0;
    //Διάβασμα του αρχείου χαρακτήρα χαρακτήρα
    //Για της ευρεσή των πλήθως των γραμμών του αρχείου
    //Και το μέγιστο μήκος των πόλεων
    while(fscanf(fileToRead, "%c", &c) == 1){
        j++;
        //Αν βρεθούν οι χαρακτήρες "-",":","\n"
        if(c=='-' || c==':' || c=='\n'){
            if(j>max){
                max=j;//Αποθήκευσει του μέγιστου μήκους στην max
            }
            //Μηδενίζει το j διότι αλλάζει η λέξη
            j=0;
            //Αυξάνει τον counter ο οποιός μετράει το πλήθος των λέξεων και αριθμών
            count++;
        }
    }
    //Επιστροφή του δείκτη στην αρχή του αρχείου
    rewind(fileToRead);
    //Το i πέρνει την τιμή των γραμμών του αρχείου
    //Υπάρχουν τρεις λέξεις σε κάθε γραμμή μαζί με τους αριθμούς 
    //όμως το count δεν έχει μετρήσει τον τελευταίο αριθμό διότι δεν υπάρχει αλλαγή γραμμής στο τέλος οπότε προσθέτει 1
    int i=(count+1)/3;
    //Δέσμευση μνήμης για την αποθήκευση του αρχείου στο struct Data
    Data root=malloc(i*sizeof(struct data));
    if(!root){
        fprintf(stderr,"Failed to allocate memory\n");
        return 1;
    }
    //Δέσμευση μνήμης για το μέγεθος των ονομάτων των πόλεων
    for(int k=0;k<i;k++){
        root[k].city1=malloc(max*sizeof(char));
        root[k].city2=malloc(max*sizeof(char));
        if(!root[k].city1 || !root[k].city2){
            fprintf(stderr,"Failed to allocate memory\n");
            return 1;
        }
    }
    int k=0;
    //Διάβασμα του αρχείου γραμμή γραμμή και αποθήκευση των στοιχείων
    while(fscanf(fileToRead, "%[^-]-%[^:]:%d\n", root[k].city1, root[k].city2, &root[k].distance) == 3){
        k++;
    }
    //Κλείσημο του αρχείου
    fclose(fileToRead);
    //Εύρεση των πόλεων με τον παρακάτω τύπο
    //Χρήση του k για το πλήθος των γραμμών αντι του i 
    //έτσι ώστε ακόμα και αν υπάρχει αλλαγή γραμμής με τα στοιχεία το πρόγραμμα να λειτουργεί
    int n=(1+my_sqrt(1+8*k))/2;
    if(n==0){  //Αν η my_sqrt γυρίσει -1 το n γίνεται 0 και το αρχείο δεν είναι σωστό
        fprintf(stderr,"Not a valid map: %s\n",argv[1]);
        return 1;
    }
    //Δημιουργία struct με τις πόλεις ως αριθμούς
    City cities=number_city(root,k);
    //Δημιουργία struct για τη σύνδεση των αριθμών με τα ονόματα των πόλεων
    City_connection index_name=inisialize_index_name(root,k,n,cities,max);
    //Απελευθέρωση του struct Data
    for(int l=0;l<i;l++){
        free(root[l].city1);
        free(root[l].city2);
    }
    free(root);
    //Ταξινόμηση του πίνακα με τους αριθμούς σε γραμμές και στήλες
    sort_city_raw(cities,k);
    sort_city_column(cities,k);
    //Εκτύπωση της βέλτιστης διαδρομής
    printf("We will visit the cities in the following order:\n");
    printf("Total cost: %d\n",Held_karp_function(cities,k,index_name,n,max));
    //Απελευθέρωση του City και City_connection
    for(int k=0;k<n;k++){
        free(index_name[k].city_name);
    }
    free(index_name);
    free(cities);

    return 0;
}