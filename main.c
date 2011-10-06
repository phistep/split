// includes
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// constants
#define NAMEBUFFERSIZE 11 // trailing \0 !
#define AMOUNTBUFFERSIZE 6 // period/comma included
#define FLOATDIGITS "6.2" // {overall length, period/comma included}.{digits after period}

// variables
struct person{
	char name[NAMEBUFFERSIZE];
	double amount;
};
struct transaction{
	char get[NAMEBUFFERSIZE];
	char give[NAMEBUFFERSIZE];
	double amount;
};
struct person * giveptr;
struct person * getptr;
struct transaction * transact; // gets allocated and enlarged
struct transaction * transactptr; // to manipulate transact
double average;
double total;
char floatbuffer[AMOUNTBUFFERSIZE]; // comma to period replacement buffer
int perscount; // number of persons
int getcount = 0; // elements in get[]
int givecount = 0; // elements in give[]
int transactcount = 0; // elements of transact[]
int i, j, k, geti, givei; // for iterizers

// function prototypes
double avg(struct person * data, size_t size);
double calctotal(struct person * data, size_t size);
void calcdiff(struct person * data, int size, double average);
void sortdesc(struct person * data, size_t size);

// main
int main(int argc, char * argv[]){
	if(argc%2 == 0 || argc < 5 || !strcmp(argv[1], "-?")){ // no even number of arguments || only one pair || help
		puts("Insufficient number of arguments! (or -?)\nEnter at least two pairs of a name followed by a number.\nE.g.:\n $ split foo 2.30 bar 4.20");
		return 1;
	}

	perscount = (argc-1)/2;
	struct person data[perscount];

	// convert argv to data structure
	for(i=0, j=1;i < perscount;i++, j+=2){		
		// comma to period replacement (for atof)
		strcpy(floatbuffer, argv[j+1]);
		for(k=0;k < AMOUNTBUFFERSIZE;k++)
			if(floatbuffer[k] == ',')
				floatbuffer[k] = '.';
				
		strcpy(data[i].name, argv[j]);
		data[i].amount = atof(floatbuffer);
	}

	// print formatted input list
	for(i=0;i<perscount;i++)
		printf("\n%" FLOATDIGITS "f %s", data[i].amount, data[i].name);

	// avg & total
	average = avg(data, perscount);
	total = calctotal(data,perscount);
		
	// splitting into get & give lists
	for(i=0;i<perscount;i++){
		if(data[i].amount > average)
			getcount++;
		if(data[i].amount < average)
			givecount++;
	}
	struct person get[getcount];
	struct person give[givecount];
	geti = getcount; givei = givecount;
	for(i=0;i<perscount;i++){
		if(data[i].amount > average){
			geti--;
			get[geti].amount = data[i].amount;
			strcpy(get[geti].name, data[i].name);
		}
		if(data[i].amount < average){
			givei--;
			give[givei].amount = data[i].amount;
			strcpy(give[givei].name, data[i].name);
		}
	}

	// calc diffs from average
	calcdiff(get, getcount, average);
	calcdiff(give, givecount, average);

	// sort descending
	sortdesc(get, getcount);
	sortdesc(give, givecount);
	
	// calculate givings
	transact = (struct transaction *) malloc(0); // create empty transaction array
	giveptr = give;
	getptr = get;
	for(i = 0; i < getcount; i++, getptr++, giveptr = give){ // cycle through getters
		do{
			if(!giveptr->amount) // advance to next giver
				giveptr++;
			else if(getptr->amount > giveptr->amount){
				getptr->amount -= giveptr->amount;
				transact = (struct transaction *) realloc(transact, ++transactcount * sizeof(struct transaction)); // enlarge transact array
				transactptr = transact + (transactcount - 1);
				transactptr->amount = giveptr->amount;
				giveptr->amount = 0;
				strcpy(transactptr->give, giveptr->name);
				strcpy(transactptr->get, getptr->name);
			}
			else{
				giveptr->amount -= getptr->amount;
				transact = (struct transaction *) realloc(transact, ++transactcount * sizeof(struct transaction)); // enlarge transact array
				transactptr = transact + (transactcount - 1);
				transactptr->amount = getptr->amount;
				getptr->amount = 0;
				strcpy(transactptr->give, giveptr->name);
				strcpy(transactptr->get, getptr->name);
			}
		} while (getptr->amount);
	}

	// print output
	printf("\n\nTotal:   %" FLOATDIGITS "f\nAverage: %" FLOATDIGITS "f\n\nResult:\n", total, average);
	for(i=0;i<transactcount;i++)
		if(transact[i].amount) // 0.00 > 0.00 workaround
			printf("%s > %.2f > %s\n",transact[i].give, transact[i].amount, transact[i].get);
	
	// exit
	free(transact);
	return 0;
}

// functions
double avg(struct person * data, size_t size){
	int count = 0;
	double total = 0;
	for(;size > 0;size--, data++){
		total += data->amount;
		count++;
	}
	return total/count;
}

double calctotal(struct person * data, size_t size){
	double total = 0;
	for(;size > 0;size--, data++)
		total += data->amount;
	return total;
}

void calcdiff(struct person *data, int size, double average){
	for(;size > 0;size--, data++){
		if(data->amount < average)
			data->amount = average - data->amount;
		else
			data->amount = data->amount - average;
	}
}

void sortdesc(struct person * data, size_t size){
	// standard bubble sort	
	size_t i;
	int swapped;
	struct person * dataptr;
	double amountbuffer;
	char namebuffer[NAMEBUFFERSIZE];

	do{
		swapped = 0;
		for(i = 0, dataptr = data; i < size - 1; i++, dataptr++){
			if(dataptr->amount < (dataptr+1)->amount){
				amountbuffer = dataptr->amount;
				dataptr->amount = (dataptr+1)->amount;
				(dataptr+1)->amount = amountbuffer;

				strcpy(namebuffer, dataptr->name);
				strcpy(dataptr->name, (dataptr+1)->name);
				strcpy((dataptr+1)->name, namebuffer);

				swapped = 1;
			}
		}
	} while (swapped);
}

