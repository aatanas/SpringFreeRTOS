
/* Standard includes. */
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>

/* FreeRTOS kernel includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"

#define printff(args...) printf(args); fflush(stdout);
#define ffprintf(strm,args...) fprintf(strm,args); fflush(strm);

#define SIZE 10

const char* const sHeur[] = {"a","C","d","T_est","d+W*C","d+W*T_est"};
/*
	H = a First Come First Served (FCFS)
	H = C Shortest Job First (SJF)
	H = d Earliest Deadline First
	H = Test Earliest Start Time First (ESTF)
	H = d +W · C EDF + SJF
	H = d +W · Test EDF + ESTF
*/

typedef struct Job
{
	int funk;
	int res[SIZE/2];
	int fin;
	TickType_t xStartTime;
	TickType_t xRunTime;
	TickType_t xDeadline;
	xTaskHandle xJobHandle;
} Job;

static FILE* fout;
Job JobBatch[SIZE];
int JobSorted[SIZE];
int iEdgeCount;
int piPred[SIZE*3];
int Resursi[SIZE/2];
int iJobCount;
int iResCount;
float w;
xTaskHandle xJobSchedHan;


static void funk0(){
	ffprintf(fout,"Pozz iz funk0!\n");
}
static void funk1(){
	ffprintf(fout,"Pozz iz funk1!\n");
}
static void funk2(){
	ffprintf(fout,"Pozz iz funk2!\n");
}
static void funk3(){
	ffprintf(fout,"Pozz iz funk3!\n");
}

static int max_test(Job* j){
	int max=j->xStartTime;
	for(int i=0;i<SIZE/2;i++){
		if(!(j->res[i])) break;
		int r = Resursi[j->res[i]-1];
		if(r>max) max=r;
	}
	return max;
}
int edf_test_cmp (Job * a, int * min) {
	int l= a->xDeadline + (w * max_test(a));
	if (l >= *min) return 0;
	return (*min=l);
}

int test_cmp (Job * a, int * min) {
	int l = max_test(a);
	if (l >= *min) return 0;
	return (*min=l);
}

int edf_sjf_cmp (Job * a, int * min) {
	int l = a->xDeadline + (w * a->xRunTime);
	if (l >= *min) return 0;
	return (*min=l);
}

int fcfs_cmp (Job * a, int * min) {
	int l = a->xStartTime;
	if (l >= *min) return 0;
	return (*min=l);
}

int sjf_cmp (Job * a, int * min) {
	int l = a->xRunTime;
	if (l >= *min) return 0;
	return (*min=l);
}

int edf_cmp (Job * a, int * min) {
	int l = a->xDeadline;
	if (l >= *min) return 0;
	return (*min=l);
}

int ima_pred(Job* jb){
	for(int i=1;i<iEdgeCount*2;i+=2){
		if(piPred[i]==jb-JobBatch && !(JobBatch+piPred[i-1])->fin){
			return 1;
		}
	}
	return 0;
}
int vJobSort(int (*comp)(Job *, int *))
{
	int start = 0;
	for(int i=0;i<iJobCount;i++){
		int min_ind=-1;
		int min=INT_LEAST32_MAX;
		for(int j=0;j<iJobCount;j++){
			if(JobBatch[j].fin || (iEdgeCount && ima_pred(JobBatch+j))) continue;
			if(comp(JobBatch+j,&min)){
				min_ind=j;
			}
		}
		if(JobBatch[min_ind].xDeadline < (start += JobBatch[min_ind].xRunTime)){
			return 0;
		}
		JobSorted[i]=min_ind;
		JobBatch[min_ind].fin++;
	}
	return 1;
}

void vWrapperTask(void* pvPara){
	Job* jb = (Job*)pvPara;
	switch(jb->funk){
		case 0: funk0(); break;
		case 1: funk1(); break;
		case 2: funk2(); break;
		case 3: funk3(); break;
	}
	vTaskDelay(jb->xRunTime);
	vTaskResume(xJobSchedHan);
	vTaskDelete(0);
}

void vJobSched(void* pvPara)
{
	TickType_t del = JobBatch[JobSorted[0]].xStartTime-xTaskGetTickCount();
	if(del>0) vTaskDelay(del);
	for(int i=0;i<iJobCount;i++){
		Job* jb = JobBatch + JobSorted[i];
		fprintf(fout,"Vreme: %u Job:%d S:%u R:%u D:%u funk%d\n",
				xTaskGetTickCount(),JobSorted[i],jb->xStartTime,jb->xRunTime,jb->xDeadline,jb->funk);
		vTaskPrioritySet(jb->xJobHandle, 2);
		vTaskResume(jb->xJobHandle);
		vTaskSuspend(0);
	}
	vTaskDelete(0);
}

void vPrintJobs(FILE* out)
{
	for(int i=0;i<iJobCount;i++){
		Job* jb=JobBatch+i;
		fprintf(out,"Job: %d\nFunkcija: funk%c\nStart: %u\nRun: %u\nEnd: %u\nRes: ",
				i,jb->funk,jb->xStartTime,jb->xRunTime,jb->xDeadline);
		for(int j=0;j<SIZE/2;j++){
			fprintf(out,"%d ",jb->res[j]);
		}
		ffprintf(out,"\n--------------\n");
	}
}
int main( void )
{
	char mod;
	int f;
	//fout = fopen("jb_log.txt","w");
	fout = stdout;
	for(int i=0;i<6;i++){
		printf("%d: %s\n",i,sHeur[i]);
	} fflush(stdout);
	mod = getchar(); fflush(stdin);
	if(mod=='4'||mod=='5'){
		printff("Unesi W: ");
		scanf("%f",&w); fflush(stdin);
	}
	printff("Fajl y|n? ");
	f = getchar(); fflush(stdin);
	if(f=='y'){
		freopen("jb.txt","r",stdin);
	}
	printff("Unesi N pa resurse: ");
	scanf("%d",&iResCount);
	for(int i=0;i<iResCount;i++){
		scanf("%d", Resursi+i);
	}
	printff("Unesi N pa poslove: ")
	scanf("%d",&iJobCount);
	for(int i=0;i<iJobCount;i++){
		Job* jb = JobBatch+i;
		int n;
		scanf("%u %u %u %d %d",&(jb->xStartTime),&(jb->xRunTime),&(jb->xDeadline),&(jb->funk),&n);
		for(int j=0;j<n;j++){
			scanf("%d",jb->res+j);
		}
	}
	printff("Predhodjenje (E) Y=n | N=0?\n");
	scanf("%d",&f);
	if(f>0){
		iEdgeCount = f;
		for(int i=0;i<f*2;i++){
			scanf("%d",piPred+i);
		}
	}
	switch(mod){
		case '0': case 'a':
			if(!vJobSort(fcfs_cmp)){
				printff("Nemoguc raspored!\n");
				return -1;
			}
			break;
		case '1': case 'C':
			if(!vJobSort(sjf_cmp)){
				printff("Nemoguc raspored!\n");
				return -1;
			}
			break;
		case '2': case 'd':
			if(!vJobSort(edf_cmp)){
				printff("Nemoguc raspored!\n");
				return -1;
			}
			break;
		case '3':
			if(!vJobSort(test_cmp)){
				printff("Nemoguc raspored!\n");
				return -1;
			}
			break;
		case '4':
			if(!vJobSort(edf_sjf_cmp)){
				printff("Nemoguc raspored!\n");
				return -1;
			}
			break;
		case '5':
			if(!vJobSort(edf_test_cmp)){
				printff("Nemoguc raspored!\n");
				return -1;
			}
			break;
		default:
			printff("Nevalja unos!\n");
			return -2;
	}

	for(int i=0;i<iJobCount;i++){
		xTaskCreate(vWrapperTask, "", configMINIMAL_STACK_SIZE, JobBatch+i, 1, &(JobBatch[i].xJobHandle));
		vTaskSuspend(JobBatch[i].xJobHandle);
	}
	xTaskCreate(vJobSched, "sched", configMINIMAL_STACK_SIZE, NULL, 3, &xJobSchedHan);
	vTaskStartScheduler();

	return 0;
}
