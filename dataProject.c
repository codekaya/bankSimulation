#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct operation_type{
	char optname[100];
	float commission;
	int optnum;
	struct operation_type *nextopt;
}operation_type;

typedef struct transaction{
	int optype;
	int tno;
	float amount;
	struct transaction *nexttr;
}transaction;

typedef struct customer{
	char fname[100];
	char lname[100];
	int cno;
	float paid_commission;
	transaction *trans;
	struct customer *nextc;
}customer;

typedef struct branch{
	char bname[100];
	int bno;
	customer *custs;
	struct branch *nextb;
}branch;

typedef struct bank{
	branch *branches;
	operation_type *optypes;
}bank;

void readOperationTypes(FILE *file, operation_type** headPointer){
	char name[100];
	float commission;
	int optNumber=0;
	operation_type* temp;
	temp = (operation_type*)malloc(sizeof(operation_type));
	
	while(!feof(file)){
		fscanf(file,"%s %f",name,&commission);
		operation_type* newNode = (operation_type*)malloc(sizeof(operation_type));
		
		strcpy(newNode->optname,name);
		newNode->commission = commission;
		newNode->optnum = ++optNumber;
		newNode->nextopt = NULL;
		if(optNumber==1){
			*headPointer = newNode;
			temp = *headPointer;
		}
		else{
			temp->nextopt =newNode ;
			temp = temp->nextopt;
		}
	}
}
void printOperationTypes(operation_type** headPointer){
	operation_type* temp = *headPointer;
	printf("optypes linked list : \n");
	while(temp != NULL){
        printf("--%d %s %.1f \n",temp->optnum,temp->optname,temp->commission);
        temp = temp->nextopt;
    }
	printf("\n");
}
void readBranches(FILE *file,branch** headPointer) {
	char name[100];
	int branchNumber=0;
	branch* temp;
	temp = (branch*)malloc(sizeof(branch));
	
	while(!feof(file)){
		fscanf(file,"%s",name);
		branch* newNode = (branch*)malloc(sizeof(branch));
		
		strcpy(newNode->bname,name);
		newNode->bno = ++branchNumber;
		newNode->custs = NULL;
		newNode->nextb = NULL;
		if(branchNumber==1){
			*headPointer = newNode;
			temp = *headPointer;
		}
		else{
			temp->nextb =newNode ;
			temp = temp->nextb;
		}
	}
}
void printBranches(branch** headPointer) {
	branch* temp = *headPointer;
	while(temp != NULL){
        printf("Branch : %d %s \n",temp->bno,temp->bname);
        temp = temp->nextb;
    }
	printf("\n");
}
void readCustomers(FILE *file,branch** headPointer) {
	char fname[100];
	char lname[100];
	int cno =0;
	int i;
	int branchNo;
	customer* temp;
	temp = (customer*)malloc(sizeof(customer));
	
	branch* tempBranch;
	tempBranch = (branch*)malloc(sizeof(branch));
	tempBranch = *headPointer;
	
	while(!feof(file)){
		fscanf(file,"%d %s %s",&branchNo,fname,lname);
		
		for(i=0;i<branchNo-1;++i){ tempBranch = tempBranch->nextb; }
		customer* newNode = (customer*)malloc(sizeof(customer));
		strcpy(newNode->fname,fname);
		strcpy(newNode->lname,lname);
		newNode->trans = NULL;
		newNode->nextc = NULL;
		newNode->paid_commission = 0;
		
		if(tempBranch->custs==NULL){ 
			tempBranch->custs = newNode;
			newNode->cno = 1;
		}
		else{
			temp = tempBranch->custs;
			while(temp->nextc!=NULL){
				temp = temp->nextc;
				cno++;		
			}
			temp->nextc = newNode;
			newNode->cno = cno+1;
		}
		tempBranch = *headPointer;
		cno=1;
	}
}

void printCustomers(branch **headPointer) {
	branch* tempBranch = *headPointer;
	customer* temp;
	printf("**********************\n");
	while(tempBranch!=NULL){
		if(tempBranch->custs!=NULL){
			temp = tempBranch->custs;
			printf("Branch : %d %s \n",tempBranch->bno,tempBranch->bname);
			while(temp!=NULL){
				printf("  --%d %s %s\n",temp->cno,temp->fname,temp->lname);
        		temp = temp->nextc;
			}
			tempBranch = tempBranch->nextb;
		}
		else{
			printf("Branch : %d %s \n",tempBranch->bno,tempBranch->bname);
			tempBranch = tempBranch->nextb;
		}
	}
	printf("\n");
}
void readTransactions(FILE *file,branch **headBranch,operation_type** headOptype) {
	branch* tempBranch;
	tempBranch = (branch*)malloc(sizeof(branch));
	tempBranch = *headBranch;
	
	customer* tempCustomer;
	tempCustomer = (customer*)malloc(sizeof(customer));
	
	operation_type* tempOptype;
	tempOptype = (operation_type*)malloc(sizeof(operation_type));
	tempOptype = *headOptype;
	
	transaction* tempTrans;
	tempTrans = (transaction*)malloc(sizeof(transaction));
	
	int branchNumber;
	int customerID;
	int i,j;
	int tno=0;
	int opType;
	float amount;
	
	while(!feof(file)){
		fscanf(file,"%d %d %d %f",&branchNumber, &customerID, &opType, &amount);
		for(i=0;i<branchNumber-1;++i){ tempBranch = tempBranch->nextb; }
		tempCustomer = tempBranch->custs;
		for(j=0;i<customerID-1;++i){ tempCustomer = tempCustomer->nextc; }
		while(tempOptype->optnum!=opType){ tempOptype = tempOptype->nextopt;}
		//printf("%s %s %d\n",tempBranch->bname,tempCustomer->fname,tempOptype->optnum);
		transaction* newNode = (transaction*)malloc(sizeof(transaction));
		newNode->amount = amount;
		newNode->nexttr = NULL;
		newNode->optype = opType;
		
		if(tempCustomer->trans==NULL){ 
			tempCustomer->trans = newNode;
			newNode->tno = 1;
		}
		else{
			tempTrans = tempCustomer->trans;
			while(tempTrans->nexttr!=NULL){
				tempTrans = tempTrans->nexttr;
				tno++;		
			}
			tempTrans->nexttr = newNode;
			newNode->tno = tno+1;
		}
		tno=1;
		
		tempBranch = *headBranch;
		tempOptype = *headOptype;
	}
}
void printTransactions(branch **headBranch,operation_type** headOptype) {
	branch* tempBranch;
	tempBranch = *headBranch;
	customer* tempCustomer;
	operation_type* tempOptype;
	tempOptype = *headOptype;
	
	transaction* tempTrans;
	
	while(tempBranch!=NULL){
		if(tempBranch->custs!=NULL){
			tempCustomer = tempBranch->custs;
			printf("Branch : %d %s \n",tempBranch->bno,tempBranch->bname);
			while(tempCustomer!=NULL){
				printf("  --%d %s %s\n",tempCustomer->cno,tempCustomer->fname,tempCustomer->lname);
        		tempTrans = tempCustomer->trans;
        		while(tempTrans!=NULL){
        			printf("\t  ++tno %d operation type (optype) %d amount %.2f\n",tempTrans->tno,tempTrans->optype,tempTrans->amount);
        			tempTrans = tempTrans->nexttr;
				}
				tempCustomer = tempCustomer->nextc;
			}
			tempBranch = tempBranch->nextb;
		}
		else{
			printf("Branch : %d %s \n",tempBranch->bno,tempBranch->bname);
			tempBranch = tempBranch->nextb;
		}
	}
	printf("\n");
}
void printPaidCommission(branch **headBranch,operation_type** headOptype) {
	branch* tempBranch;
	tempBranch = *headBranch;
	customer* tempCustomer;
	operation_type* tempOptype;
	tempOptype = *headOptype;
	transaction* tempTrans;
	float totalCom = 0;
	
	printf("*********************************\n\n");
	while(tempBranch!=NULL){
		if(tempBranch->custs!=NULL){
			tempCustomer = tempBranch->custs;
			printf("BRANCH : %d %s \n\n",tempBranch->bno,tempBranch->bname);
			while(tempCustomer!=NULL){
					if(tempCustomer->trans!=NULL){
						printf("-->cust id %d : %s %s\n",tempCustomer->cno,tempCustomer->fname,tempCustomer->lname);
		        		tempTrans = tempCustomer->trans;
		        		while(tempTrans!=NULL){
		        			totalCom+=tempTrans->amount;
		        			printf("-- tno %d optype %d commission rate  amount %.2f\n",tempTrans->tno,tempTrans->optype,tempTrans->amount);
		        			tempTrans = tempTrans->nexttr;
						}
						printf("** paid commission %.2f",totalCom);
						tempCustomer = tempCustomer->nextc;
					}
					else{
						printf("-->cust id %d : %s %s\n",tempCustomer->cno,tempCustomer->fname,tempCustomer->lname);
						printf("  --(customer has no transaction)\n  **paid commission 0.00\n");
						tempCustomer = tempCustomer->nextc;
					}
			}
			tempBranch = tempBranch->nextb;
		}
		else{
			printf("BRANCH : %d %s \n\n",tempBranch->bno,tempBranch->bname);
			tempBranch = tempBranch->nextb;
		}
	}
	printf("\n");
}

int main(){
	
	FILE* file;
	int choice;
	char fileName[100];
	
	bank* Banka = (bank*)malloc(sizeof(bank));
	Banka->branches= NULL;
	Banka->optypes = NULL;
	
	while(1){
		printf("1) Read operation types from the file\n"
			   "2) Read branches from the file\n"
			   "3) Read customers from the file\n"
			   "4) Read customer transactions from the file\n"
			   "5) Calculate paid commission amount of each customers in each branches\n"
			   "\noption : ");
		
		if(scanf("%d", &choice) && !(choice<=5 && choice>=1)){ break;}
		
		if(!(choice==5)){
			printf("Please enter the name of the input file :\n");
			scanf("%s", fileName);
			printf("\n");
			file = fopen(fileName, "r");
		}
		
		if(choice==1){
			readOperationTypes(file,&(Banka->optypes));
			printOperationTypes(&Banka->optypes);
			
		}
		else if(choice==2){
			readBranches(file,&(Banka->branches));
			printBranches(&Banka->branches);
		}
		else if(choice==3){
			readCustomers(file,&Banka->branches);
			printCustomers(&Banka->branches);
		}
		else if(choice==4){
			readTransactions(file,&Banka->branches,&Banka->optypes);
			printTransactions(&Banka->branches,&Banka->optypes);
		}
		else if(choice==5){
			printPaidCommission(&Banka->branches,&Banka->optypes);
		}
		else{ break;}
		
		fclose(file);
		
	}
	return 0;
}
