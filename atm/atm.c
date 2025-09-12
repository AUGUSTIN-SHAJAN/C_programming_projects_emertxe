//Author: Augustin
//Date: 5/7/25

#include  <stdio.h>
int main(int argc , char **argv)
	{
	int pin=1000,money=0,atm_hold;
	printf("\e[1;1H\e[2J");
	printf("Enter Pin:");

	while(1)
		{
			scanf("%d",&atm_hold);
			printf("\e[1;1H\e[2J");
			if(atm_hold==pin)
				{
					printf("Welcome to D ATM");
					break;
				}
				
			else
				{
					printf("Enter the correct pin:");

				}
			}
	
	while(1)
		{
			
			printf("\n\n\n1.Withdraw\n");
			printf("2.Check balance\n");
			printf("3.Change pin\n");
			printf("4.Deposit\n");
			printf("5.Exit\n");
			printf("\nEnter option:");
			scanf("%d",&atm_hold);
			printf("\e[1;1H\e[2J");

			if (atm_hold==5)
				{	
					printf("Bye\n");
					break;
				}
				

			switch (atm_hold)
				{
					case 1:	
						printf("Enter the amount:");
						scanf("%d",&atm_hold);
						printf("\e[1;1H\e[2J");
						if(atm_hold>money)
							{
								printf("inadequate amount in account");
							}
						else if (atm_hold<1) 
							{
								printf("Retry");
							}	
						else
							{
								money=money-atm_hold;
								printf("Success");
							}				
					break;

					case 2:
						printf("Account Balance: %d",money);
					break;

					case 3:
						printf("Enter new Pin:");
						scanf("%d",&pin);
						printf("\e[1;1H\e[2J");
						printf("Pin changed");
					break;
					
					case 4:
						printf("Enter amount:");
						scanf("%d",&atm_hold);
						printf("\e[1;1H\e[2J");

						if (atm_hold<1)
							{
								printf("Retry");
							}
						else
							{
								money=money+atm_hold;
								printf("Success");
							}
						
					break;

					default:
					printf("Invalid Option");
					break;
				}
	}


	return 0;
	}