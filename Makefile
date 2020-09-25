BuildRunServer: 	
		docker build -t tarea1 . && docker run -it -p 8585:8585 -v $(PWD)/Resultados:/home/Tarea1/Resultados tarea1

BuildClient:
		gcc -o cliente Cliente.c 