#include <stdio.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <stdio.h>      //Printf
#include <stdint.h> 

//Defines internos necesarios
#define HEIGHT_DIS 600
#define WIDTH_DIS 400
#define WIDTH_SYMS 100
#define WIDTH_CRUZ 50
#define HEIGHT_SYMS 100
#define COL1 75
#define FINCOL1 (COL1 + WIDTH_SYMS)
#define COL2 (FINCOL1 + 50)
#define FINCOL2 (COL2 + WIDTH_SYMS)

//Para trabajar en el arreglo
#define TAMCOLAEVENTO 100
enum {PLAY, RESTART, MENU, SETTINGS, QUIT};

//Necesario para debug
#define DEBUG 1


#define false 0
#define true 1

char cola_ev[TAMCOLAEVENTO];        //Cola de eventos
char* pevento = cola_ev; //PUNTERO EN DONDE SE AGREGAN LOS EVENTOS

//variables necesarias para allegro
	static ALLEGRO_DISPLAY * display = NULL;       //Creo display
	static ALLEGRO_BITMAP * display_background;    //Fondo
	static ALLEGRO_BITMAP * play;                  //Boton play
	static ALLEGRO_BITMAP * controles;             //controles para mostrar
	static ALLEGRO_BITMAP * restart;               //boton restart
	static ALLEGRO_BITMAP * settings;              //bton settings
	static ALLEGRO_BITMAP * menu;                  //Boton menu
	static ALLEGRO_BITMAP * quit;                  //bton quit
	static ALLEGRO_BITMAP * tetris;                //titulo
	static ALLEGRO_BITMAP * max_score;             //boton max score
	static ALLEGRO_BITMAP * rules;                 //boton rules
        static ALLEGRO_EVENT_QUEUE  * event_queue = NULL;   //creo cola de eventos
        static char close_display = false;             //Variable para ver si cerrar el display
        
//Funciones internas
static void config_menu (void);

int main(void) 
{
    char controles_juego = false;
     //Inicializo Allegro
	if( !al_init() ) //lo mismo que igual a 0
	{
		fprintf(stderr, "Unable to start allegro\n");
		return -1;
	}
        
        
        //Inicializo cola de eventos
        event_queue = al_create_event_queue();
        if(!event_queue) 
        {
            al_uninstall_system();
            printf("failed to create event_queue!\n");
            return -1;
        }
    
        //Inicializo teclado
        if(!al_install_keyboard()) 
        {
            printf("failed to initialize the keyboard!\n");
            al_destroy_event_queue(event_queue);
            al_uninstall_system();
            return -1;
        }
        
         //Inicializo Mouse
    
        if (!al_install_mouse())
        {
            printf ("failed to inicialize mouse!\n");
            al_destroy_event_queue(event_queue);
            al_uninstall_system();
            return -1;
        }
        
        
        //Inicializo configuracion de imagenes
	else if( !al_init_image_addon())
	{
		fprintf(stderr,"Unable to start image addon \n"); //Igual que printf pero imprime al error std 
		al_uninstall_system();
                al_destroy_event_queue(event_queue);
		return -1;
	}
        
        //Cargo bitmaps
        
        //Cargo fondo
	else if( !(display_background = al_load_bitmap("fondo2.jpg")) )
	{
		fprintf(stderr,"Unable to load background\n"); 
		al_uninstall_system();
		al_shutdown_image_addon();
                al_destroy_event_queue(event_queue);
		return -1;
	}
        
        //cargo boton play
	else if( !(play = al_load_bitmap("play.png")) ) 
	{
		fprintf(stderr,"Unable to load play\n"); 
                al_destroy_bitmap(display_background);
		al_uninstall_system();
		al_shutdown_image_addon();
                al_destroy_event_queue(event_queue);
		return -1;
	}
        //Cargo quit
        else if( !(quit = al_load_bitmap("quit.png")) ) 
	{
		fprintf(stderr,"Unable to load quit\n"); 
                al_destroy_bitmap(display_background);
                al_destroy_bitmap(play);
		al_uninstall_system();
		al_shutdown_image_addon();
                al_destroy_event_queue(event_queue);
		return -1;
	}
        //cargo restart
        else if( !(restart = al_load_bitmap("restart.png")) ) 
	{
		fprintf(stderr,"Unable to load restart\n"); 
                al_destroy_bitmap(display_background);
                al_destroy_bitmap(play);
                al_destroy_bitmap(quit);
		al_uninstall_system();
		al_shutdown_image_addon();
                al_destroy_event_queue(event_queue);
		return -1;
	}
        //Cargo max_score
        else if( !(max_score = al_load_bitmap("maxscore.png")) ) 
	{
		fprintf(stderr,"Unable to load max score\n"); 
                al_destroy_bitmap(display_background);
                al_destroy_bitmap(play);
                al_destroy_bitmap(quit);
                al_destroy_bitmap(restart);
		al_uninstall_system();
		al_shutdown_image_addon();
                al_destroy_event_queue(event_queue);
		return -1;
	}
        //Cargo rules
        else if( !(rules = al_load_bitmap("rules.png")) ) 
	{
		fprintf(stderr,"Unable to load rules\n"); 
                al_destroy_bitmap(display_background);
                al_destroy_bitmap(play);
                al_destroy_bitmap(quit);
                al_destroy_bitmap(restart);
                al_destroy_bitmap(max_score);
		al_uninstall_system();
		al_shutdown_image_addon();
                al_destroy_event_queue(event_queue);
		return -1;
	}
        //cargo settings
        else if( !(settings = al_load_bitmap("settings.png")) ) 
	{
		fprintf(stderr,"Unable to load settings\n"); 
                al_destroy_bitmap(display_background);
                al_destroy_bitmap(rules);
                al_destroy_bitmap(play);
                al_destroy_bitmap(quit);
                al_destroy_bitmap(restart);
                al_destroy_bitmap(max_score);
		al_uninstall_system();
		al_shutdown_image_addon();
                al_destroy_event_queue(event_queue);
		return -1;
	}
        //Cargo menu
        else if( !(menu = al_load_bitmap("menu.png")) ) 
	{
		fprintf(stderr,"Unable to load menu\n"); 
                al_destroy_bitmap(display_background);
                al_destroy_bitmap(rules);
                al_destroy_bitmap(play);
                al_destroy_bitmap(quit);
                al_destroy_bitmap(restart);
                al_destroy_bitmap(max_score);
                al_destroy_bitmap(settings);
		al_uninstall_system();
		al_shutdown_image_addon();
                al_destroy_event_queue(event_queue);
		return -1;
	}
        
        //Cargo tetris
	else if( !(tetris = al_load_bitmap("Menu.png")) )
	{
		fprintf(stderr,"Unable to load tetris\n"); 
                al_destroy_bitmap(display_background);
                al_destroy_bitmap(rules);
                al_destroy_bitmap(play);
                al_destroy_bitmap(quit);
                al_destroy_bitmap(restart);
                al_destroy_bitmap(max_score);
                al_destroy_bitmap(settings);
                al_destroy_bitmap(menu);
		al_uninstall_system();
		al_shutdown_image_addon();
                al_destroy_event_queue(event_queue);
		return -1;
	}
        //Cargo controles de juego
        else if( !(controles = al_load_bitmap("Controles de juego.png")) )
	{
		fprintf(stderr,"Unable to load Controles de juego\n"); 
                al_destroy_bitmap(display_background);
                al_destroy_bitmap(rules);
                al_destroy_bitmap(play);
                al_destroy_bitmap(quit);
                al_destroy_bitmap(restart);
                al_destroy_bitmap(max_score);
                al_destroy_bitmap(settings);
                al_destroy_bitmap(menu);
                al_destroy_bitmap(tetris);
		al_uninstall_system();
		al_shutdown_image_addon();
                al_destroy_event_queue(event_queue);
		return -1;
	}
        //Inicializo display
	else if( !(display = al_create_display(WIDTH_DIS,HEIGHT_DIS)) )
	{
		fprintf(stderr,"Unable to create display\n"); 
		al_destroy_bitmap(display_background);
                al_destroy_bitmap(rules);
                al_destroy_bitmap(play);
                al_destroy_bitmap(quit);
                al_destroy_bitmap(tetris);
                al_destroy_bitmap(restart);
                al_destroy_bitmap(max_score);
                al_destroy_bitmap(settings);
                al_destroy_bitmap(menu);
                al_destroy_bitmap(controles);
		al_uninstall_system();
		al_shutdown_image_addon();
                al_destroy_event_queue(event_queue);
		return -1;
	}
       
        
        //Incluyo los eventos a la cola de eventos
        al_register_event_source(event_queue, al_get_display_event_source(display));
        al_register_event_source(event_queue, al_get_keyboard_event_source());
        al_register_event_source(event_queue, al_get_mouse_event_source());
        
        
        //Dibujo en el display 
        config_menu();
        
        while (!close_display)
        {
            ALLEGRO_EVENT evento;   //Creo variable para menejar eventos
            if (al_get_next_event(event_queue, &evento))
            {
                if (evento.type == ALLEGRO_EVENT_DISPLAY_CLOSE) //Verifico si cerrar el display
                    close_display = true;
                if (evento.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP)
                {
                    if ((evento.mouse.x >= COL1) && (evento.mouse.x <= FINCOL1) )   //Estoy en la primer columna
                    {   
                        //Boton play, debe volver al estado jugando, limpiando la cola de eventos anteriores
                        //Pero el estado de la matriz de juego sigue igual 
                        //LA MAQUINA DE ESTADOS DEBE ABRIR EL DISPLAY DE JUEGO
                        //Tambien debo cerrar el display del menu 
                        if ((evento.mouse.y >= HEIGHT_SYMS) && (evento.mouse.y <= HEIGHT_SYMS*2))
                        {
                           *pevento++ = PLAY; //Pongo en el arreglo que tocaron el boton play y corro el puntero
                           close_display = true;    //Para abrir la pantalla de juego real
                           #if DEBUG
                            printf("boton play\n");
                            #endif
                        }
                        //Boton rules
                        //no envia ninguna senal a la maquina de estados, solo abre otro display para mostrar las 
                        //Instrucciones
                        if ((evento.mouse.y >= (HEIGHT_SYMS*2 + 50)) && (evento.mouse.y <= ((HEIGHT_SYMS*2 + 50) + HEIGHT_SYMS)))
                        {
                            al_draw_scaled_bitmap(controles,
						0,0, al_get_bitmap_width(controles),al_get_bitmap_height(controles), //imagen
						0,0,al_get_display_width(display),al_get_display_height(display), //a que tamaño queres que se dibuje la imagen
						0); //SIn flags podrian usar ALLEGRO_FLIP_HORIZONTAL o ALLEGRO_FLIP_VERTICAL muy utiles
                            al_draw_scaled_bitmap(quit,
                                                0,0, al_get_bitmap_width(quit),al_get_bitmap_height(quit),
                                                WIDTH_DIS-WIDTH_CRUZ ,0 ,WIDTH_CRUZ,WIDTH_CRUZ,  // POSICION. tamano que quiero que se imprima la imagen
                                                0); //rotacion
                            al_flip_display();
                            
                            controles_juego = true;
                            #if DEBUG
                            printf("boton rules\n");
                            #endif
                        }
                        //Boton quit
                        //Envio evento ya que debe terminar el programa
                        //Cierro el display
                        if ((evento.mouse.y >= (HEIGHT_SYMS*3 + 100)) && (evento.mouse.y <= ((HEIGHT_SYMS*3 + 100) + HEIGHT_SYMS)))
                        {
                            *pevento++ = QUIT;
                            close_display = true;   //Se cierra el frontend
                            #if DEBUG
                            printf("boton QUIT\n");
                            #endif
                        }
                    }
                    
                    if ((evento.mouse.x >= COL2) && (evento.mouse.x <= FINCOL2) )   //Estoy en la segunda columna
                    {
                        //Boton restart
                        //Se debe abrir algo para mostrar las opciones de juego
                        if ((evento.mouse.y >= HEIGHT_SYMS) && (evento.mouse.y <= HEIGHT_SYMS*2))
                        {
                           *pevento++ = SETTINGS; //Pongo en el arreglo que tocaron el boton play y corro el puntero
                           #if DEBUG
                            printf("boton SETTINGS\n");
                            #endif
                        }
                        //Boton max_score
                        //No se debe mandar un evento, solo afecta al front end. debe abrir el archivo que muestr
                        //La puntuacion maxima
                        if ((evento.mouse.y >= (HEIGHT_SYMS*2 + 50)) && (evento.mouse.y <= ((HEIGHT_SYMS*2 + 50) + HEIGHT_SYMS)))
                        {
                            //Debo ver como mostrar el puntaje maximo
                            #if DEBUG
                            printf("boton puntaje maximo\n");
                            #endif
                        }
                    }
                }
                if (controles_juego == true)
                {
                  if (evento.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP)
                            {
                                if (evento.mouse.x >= WIDTH_DIS-WIDTH_SYMS)
                                    if (evento.mouse.y <= HEIGHT_SYMS)
                                    {
                                      config_menu(); 
                                    }
                            }
                    
                }
            }
            
            
	
	
        }
        
        al_destroy_bitmap(display_background);
        al_destroy_bitmap(rules);
        al_destroy_bitmap(play);
        al_destroy_bitmap(quit);
        al_destroy_bitmap(restart);
        al_destroy_bitmap(max_score);
        al_destroy_bitmap(tetris);
        al_destroy_bitmap(settings);
        al_destroy_bitmap(menu);
	al_shutdown_image_addon();
        al_destroy_event_queue(event_queue);
        al_destroy_display(display);
        return 0;
}


static void config_menu (void)
{

	//Dibujo en el display
        
        al_clear_to_color(al_map_rgb(0,0,0));   //pongo el fondo real en negro
	//Agrego una imagen para hacer el fondo del menu
        al_draw_scaled_bitmap(display_background,
						0,0, al_get_bitmap_width(display_background),al_get_bitmap_height(display_background), //imagen
						0,0,al_get_display_width(display),al_get_display_height(display), //a que tamaño queres que se dibuje la imagen
						0); //SIn flags podrian usar ALLEGRO_FLIP_HORIZONTAL o ALLEGRO_FLIP_VERTICAL muy utiles
        //Agrego tetris
        al_draw_scaled_bitmap(tetris,
						0,0, al_get_bitmap_width(tetris),al_get_bitmap_height(tetris), //imagen
						0,0,400,100, //a que tamaño queres que se dibuje la imagen
						0); //SIn flags podrian usar ALLEGRO_FLIP_HORIZONTAL o ALLEGRO_FLIP_VERTICAL muy utiles
        //Agrego boton play
        al_draw_scaled_bitmap(play,
            0,0, al_get_bitmap_width(play),al_get_bitmap_height(play),
            COL1,HEIGHT_SYMS ,WIDTH_SYMS,HEIGHT_SYMS,  // POSICION. tamano que quiero que se imprima la imagen
            0); //rotacion
        //Agrego boton settings
        al_draw_scaled_bitmap(settings,
            0,0, al_get_bitmap_width(settings),al_get_bitmap_height(settings),
            COL2,HEIGHT_SYMS ,WIDTH_SYMS,HEIGHT_SYMS,  // POSICION. tamano que quiero que se imprima la imagen
            0); //rotacion
        
        //Agrego boton max score
        al_draw_scaled_bitmap(max_score,
            0,0, al_get_bitmap_width(max_score),al_get_bitmap_height(max_score),
            COL2, HEIGHT_SYMS*2 + 50 ,WIDTH_SYMS,HEIGHT_SYMS,  // POSICION. tamano que quiero que se imprima la imagen
            0); //rotacion

        //Agrego boton rules
        al_draw_scaled_bitmap(rules,
            0,0, al_get_bitmap_width(rules),al_get_bitmap_height(rules),
            COL1,HEIGHT_SYMS*2 + 50 ,WIDTH_SYMS,HEIGHT_SYMS,  // POSICION. tamano que quiero que se imprima la imagen
            0); //rotacion
        
        
        //Agrego boton quit
        al_draw_scaled_bitmap(quit,
            0,0, al_get_bitmap_width(quit),al_get_bitmap_height(quit),
            COL1,HEIGHT_SYMS*3 + 100 ,WIDTH_SYMS,HEIGHT_SYMS,  // POSICION. tamano que quiero que se imprima la imagen
            0); //rotacion
        
        
	al_flip_display();
}