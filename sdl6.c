/**
 * hello7_mouse.c - the sprite follows the cursor, or is chased if you click
 */
//
#include <stdbool.h>
#include <stdio.h>
#include <math.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_image.h>

#define WINDOW_WIDTH (640)
#define WINDOW_HEIGHT (480)

// speed in pixels/second
#define SPEED (300)

int main(void)
{
    // attempt to initialize graphics and timer system
    if (SDL_Init(SDL_INIT_VIDEO|SDL_INIT_TIMER) != 0)
    {
        printf("error initializing SDL: %s\n", SDL_GetError());
        return 1;
    }

    SDL_Window* win = SDL_CreateWindow("Hello, CS50!",
                                       SDL_WINDOWPOS_CENTERED,
                                       SDL_WINDOWPOS_CENTERED,
                                       WINDOW_WIDTH, WINDOW_HEIGHT,0);
    if (!win)
    {
        printf("error creating window: %s\n", SDL_GetError());
        SDL_Quit();
	    return 1;
    }

    SDL_Renderer *rend = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!rend)
    {
      printf("error creating renderer: %s\n", SDL_GetError());
      SDL_DestroyWindow(win);
      SDL_Quit();
      return 1;
    }
    
    SDL_Texture * tex = SDL_CreateTexture(rend, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STATIC, WINDOW_WIDTH, WINDOW_HEIGHT);
    
    SDL_Rect rects[16][16];
    int i, j, l, x, y, r;
    x = WINDOW_WIDTH / 2;
    y = 5;
    
    //create keyboard
    SDL_Rect key[10];
    bool keys[10];
    r = 3;
    for (i = 0; i < 10; i++) {
	key[i].w = WINDOW_WIDTH / 40;
	key[i].h = WINDOW_HEIGHT / 30;
	key[i].x = x;
	key[i].y = y;
	x = x + WINDOW_WIDTH / 40 + 1;
	if (r == 3) {
		y = y + WINDOW_HEIGHT / 30 + 1;
		r = 0;
		x = WINDOW_WIDTH / 2;
	}
	r++;
	keys[i] = 0;
    }
    
    //create butt's; [0] - save [1] - clean 
    SDL_Rect but[2];
    but[0].w = WINDOW_WIDTH / 4;
    but[0].h = WINDOW_HEIGHT / 4;
    but[0].x = WINDOW_WIDTH / 2;
    but[0].y = WINDOW_HEIGHT / 4;

    but[1].w = WINDOW_WIDTH / 4;
    but[1].h = WINDOW_HEIGHT / 4;
    but[1].x = WINDOW_WIDTH / 2;
    but[1].y = WINDOW_HEIGHT / 2 + 10;
    //create pixs
    bool pix[16][16];
    x = 0; y = 0;
    for (i = 0; i < 16; i++) {
	for (j = 0; j < 16; j++) {
		rects[i][j].w = WINDOW_WIDTH / 40;
    		rects[i][j].h = WINDOW_HEIGHT / 30;
    		rects[i][j].x = x;
    		rects[i][j].y = y;
    		x = x + WINDOW_WIDTH / 40 + 1;
    		pix[i][j] = 0;
    		//printf("%i %i", i, j);
	}
	y = y + WINDOW_HEIGHT / 30 + 1;
	x = 0;
    }

    int mouse_x, mouse_y, buttons;

    // animation loop
    int close_requested = 0;
    while (!close_requested)
    {
        // process events
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                close_requested = 1;
            }
        }
        
        // coloring screen
        SDL_SetRenderDrawColor(rend, 255, 255, 255, 255);
    	SDL_RenderClear(rend);
    	
	//color keys
	for (i = 0; i < 10; i++) {
		//draw pixs
		if ( keys[i] == 0 ) SDL_SetRenderDrawColor(rend, 255, 240, 240, 240);
		else SDL_SetRenderDrawColor(rend, 255, 0, 0, 0);
		SDL_RenderFillRect( rend, &key[i]);
		
		//if mlb not pressed - skipidi
		if ( !(buttons & SDL_BUTTON(SDL_BUTTON_LEFT)) ) continue;
		//if x 'n' y of mouse less than of pixel's one's - skipidi
		if ( key[i].x > mouse_x ) continue;
		//
		if ( key[i].y > mouse_y ) continue;
		//if sum of x 'n' w of mouse more than of pixs one's - skipidi
		if ( key[i].x + key[i].w < mouse_x ) continue;
		//if sum of y 'n' h of mouse more than of pixs one's - skipidi
		if ( key[i].y + key[i].h < mouse_y ) continue;
		keys[i] = 1;
	}
	//coloring rects
	buttons = SDL_GetMouseState(&mouse_x, &mouse_y);
    	for (i = 0; i < 16; i++) {
		for (j = 0; j < 16; j++) {
			//draw pixs
			if ( pix[i][j] == 0 ) SDL_SetRenderDrawColor(rend, 255, 240, 240, 240);
			else SDL_SetRenderDrawColor(rend, 255, 0, 0, 0);
			SDL_RenderFillRect( rend, &rects[i][j]);
			
			//if mlb not pressed - skipidi
			if ( !(buttons & SDL_BUTTON(SDL_BUTTON_LEFT)) ) continue;
			//if x 'n' y of mouse less than of pixel's one's - skipidi
			if ( rects[i][j].x > mouse_x ) continue;
			//
			if ( rects[i][j].y > mouse_y ) continue;
			//if sum of x 'n' w of mouse more than of pixs one's - skipidi
			if ( rects[i][j].x + rects[i][j].w < mouse_x ) continue;
			//if sum of y 'n' h of mouse more than of pixs one's - skipidi
			if ( rects[i][j].y + rects[i][j].h < mouse_y ) continue;
			pix[i][j] = 1;
		}
	}
	for( i = 0; i < 2; i++){
		 //draw butt's
		 SDL_SetRenderDrawColor(rend, 255, 240, 240, 240);
		 SDL_RenderFillRect( rend, &but[i]);
		 
		 //if butt's pressed
		 if ( !(buttons & SDL_BUTTON(SDL_BUTTON_LEFT)) ) continue;
		 //if x 'n' y of mouse less than of pixel's one's - skipidi
		 if ( but[i].x > mouse_x ) continue;
		 // y
		 if ( but[i].y > mouse_y ) continue;
		 //if sum of x 'n' w of mouse more than of pixs one's - skipidi
		 if ( but[i].x + but[i].w < mouse_x ) continue;
		 //if sum of y 'n' h of mouse more than of pixs one's - skipidi
		 if ( but[i].y + but[i].h < mouse_y ) continue;
		 if ( i == 0 ){
		 	for ( j = 0; j < 16; j++ ){
		 		for ( l = 0; l < 16; l++ ){
		 			pix[j][l] = 0;
		 		}
		 	}
		 	SDL_Delay(150);
		 }
		 else{
		    FILE *f;
		    //get name of file
		    f = fopen("pics/num.txt", "r");
		    int content;
		    fscanf(f, "x: %d", &content);
		    fclose(f);
		    printf("%d.txt have saved\n", content + 1 );
		    //++ number
		    f = fopen("pics/num.txt", "w");
		    content++;
		    fprintf(f, "x: %i", content );
		    fclose(f);
		    
		    // Create a file
		    char str[100], sum[100];
		    sprintf( str, "%d", content );
		    strcpy( sum, "pics/");
		    strcat( sum, str);
		    strcat( sum, ".txt" );
		    
		    f = fopen( sum, "w");
		    for ( i = 0; i < 16; i++ ){
		    	for ( j = 0; j < 16; j++){
		    		fprintf(f, "%i", pix[i][j] );
		    	}
		    	fprintf(f, "\n");
		    }
		    fclose(f);
		    //pass num
		    for ( i = 0; i < 10; i++ ){
		    	if ( keys[i] == true ) break;
		    }
		    f = fopen( sum, "a");
		    fprintf( f, "%i", i );
		    // Close the file
		    fclose(f);
		    SDL_Delay(150);
		}
	}
        
	// update screen stuff
        SDL_RenderPresent(rend);
        SDL_Delay(1000/60);
        
    }
    
    // clean up resources before exiting
    SDL_DestroyTexture(tex);
    SDL_DestroyRenderer(rend);
    SDL_DestroyWindow(win);
    SDL_Quit();
}
