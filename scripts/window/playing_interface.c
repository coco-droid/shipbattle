#include "../../headers/load_text.h"
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>  // Inclure SDL_ttf pour le rendu du texte
#include <stdio.h>
#include <string.h>
#include "../../headers/window/options_play.h"
#include <time.h>
#include "../../headers/events.h"
#include "../../headers/log.h"
#include "../../headers/player.h"
#include "../../headers/logics.h"
#include "../../headers/window/place_boat.h"
#include "../../headers/window/startup.h"
#include "../../headers/computer_mode.h"
#include "../../headers/graphics.h"
#include "../../headers/damier.h"
#include "../../headers/computer_mode.h"
#include "../../headers/ai.h"
#define MAX_SHOTS 4
// Variables pour la gestion des tirs
int selected_cells[MAX_SHOTS][2] = {{-1, -1}, {-1, -1}, {-1, -1}, {-1, -1}};
Grid radar_grid;
Grid play_grid;
int num_shots = 0;
int score_shots=0;

void DrawSelectedCells(Grid* grid, int selected_cells[MAX_SHOTS][2], int num_shots, SDL_Texture* targetTexture) {
    // Dessiner chaque cellule sélectionnée
    for (int i = 0; i < num_shots; i++) {
        SDL_Rect selectedRect = {grid->xPos + selected_cells[i][0] * grid->cellSize, 
                                 grid->yPos + selected_cells[i][1] * grid->cellSize, 
                                 grid->cellSize, grid->cellSize};

        // Assurez-vous que l'alpha est réinitialisé pour être complètement opaque
        SDL_SetTextureAlphaMod(targetTexture, 255);
        SDL_RenderCopy(grid->renderer, targetTexture, NULL, &selectedRect);
    }
}

void DrawFilledCircle(SDL_Renderer* renderer, int centerX, int centerY, int radius) {
    for (int w = 0; w < radius * 2; w++) {
        for (int h = 0; h < radius * 2; h++) {
            int dx = radius - w; // Décalage horizontal
            int dy = radius - h; // Décalage vertical
            if ((dx*dx + dy*dy) <= (radius * radius)) {
                SDL_RenderDrawPoint(renderer, centerX + dx, centerY + dy);
            }
        }
    }
}
void GoToHomeScreen(){
     printf("return to home screen");
     OpenMenuWindow(second_window,second_renderer,&first_window,&first_renderer);
     ShowStartupMenu(first_window,first_renderer);
     
}
void ReplayGame(){
    printf("replay the game");
    memset(hint_player_one_grid, 0, sizeof(hint_player_one_grid));
    memset(hint_player_two_grid, 0, sizeof(hint_player_two_grid));
    memset(player_one_grid, 0, sizeof(player_one_grid));
    memset(player_two_grid, 0, sizeof(player_two_grid));
    OpenMenuWindow(second_window,second_renderer,&first_window,&first_renderer);
    Computer_mode(first_window,first_renderer);
}
// Fonction pour dessiner le widget de victoire
void DrawWinWidget(SDL_Window* window, SDL_Renderer* renderer, const char* player_status, const char* winner_name, int num_shots, int num_hits, double duration, int final_score) {
    // Obtenir les dimensions de la fenêtre
    int window_width, window_height;
    SDL_GetWindowSize(window, &window_width, &window_height);

    // Activer le mélange alpha pour la transparence
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

    // Définir les dimensions du rectangle principal (60% de la largeur et de la hauteur de la fenêtre)
    int rect_width = window_width * 0.6;
    int rect_height = window_height * 0.6;
    int rect_x = (window_width - rect_width) / 2;
    int rect_y = (window_height - rect_height) / 2;

    // Choisir la couleur du rectangle en fonction du statut du joueur
    if (strcmp(player_status, "win") == 0) {
        // Couleur verte pour la victoire
        SDL_SetRenderDrawColor(renderer, 0, 128, 0, 200); // Vert semi-transparent
    } else {
        // Couleur rouge pour la défaite
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 200); // Rouge semi-transparent
    }

    SDL_Rect rect = {rect_x, rect_y, rect_width, rect_height};
    SDL_RenderFillRect(renderer, &rect);

    // Diviser le rectangle en deux parties : gauche et droite
    int left_rect_width = rect_width / 2;
    int left_rect_height = rect_height;
    int left_rect_x = rect_x;
    int left_rect_y = rect_y;
    SDL_Rect left_rect = {left_rect_x, left_rect_y, left_rect_width, left_rect_height};

    int right_rect_width = rect_width / 2;
    int right_rect_height = rect_height;
    int right_rect_x = rect_x + left_rect_width;
    int right_rect_y = rect_y;
    SDL_Rect right_rect = {right_rect_x, right_rect_y, right_rect_width, right_rect_height};

    // Charger et afficher l'image dans la partie gauche
    SDL_Texture* left_image = NULL;
    if (strcmp(player_status, "win") == 0) {
        left_image = IMG_LoadTexture(renderer, "medias/images/dialog_1.jpeg"); // Image pour la victoire
    } else {
        left_image = IMG_LoadTexture(renderer, "medias/images/dialog_2.jpeg"); // Image pour la défaite
    }

    if (left_image) {
        SDL_RenderCopy(renderer, left_image, NULL, &left_rect);
    } else {
        printf("Erreur de chargement de l'image de gauche : %s\n", SDL_GetError());
    }

    // Initialiser SDL_ttf si ce n'est pas déjà fait
    if (TTF_WasInit() == 0) {
        if (TTF_Init() == -1) {
            printf("Erreur d'initialisation de TTF : %s\n", TTF_GetError());
            return;
        }
    }

    // Charger une police
    TTF_Font* font = TTF_OpenFont("medias/font/Sora.ttf", 28); // Augmenter la taille de la police
    if (!font) {
        printf("Erreur de chargement de la police : %s\n", TTF_GetError());
        return;
    }

    // Créer le texte à afficher
    SDL_Color textColor = {255, 255, 255, 255}; // Couleur blanche
    char display_text[100];

    if (strcmp(player_status, "win") == 0) {
        snprintf(display_text, sizeof(display_text), "You Win!");
    } else {
        snprintf(display_text, sizeof(display_text), "You Lose!\n%s Wins!", winner_name);
    }

    // Gérer plusieurs lignes de texte
    char* line = strtok(display_text, "\n");
    int line_count = 0;
    int total_text_height = 0;

    int text_y = right_rect_y + right_rect_height * 0.05; // Commencer à 5% de la hauteur du rectangle droit

    while (line != NULL) {
        SDL_Surface* textSurface = TTF_RenderText_Blended(font, line, textColor);
        if (!textSurface) {
            printf("Erreur de rendu du texte : %s\n", TTF_GetError());
            TTF_CloseFont(font);
            return;
        }

        SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
        SDL_FreeSurface(textSurface);

        int text_width, text_height;
        SDL_QueryTexture(textTexture, NULL, NULL, &text_width, &text_height);

        SDL_Rect textRect = {
            right_rect_x + (right_rect_width - text_width) / 2,
            text_y + line_count * (text_height + 5),
            text_width,
            text_height
        };

        SDL_RenderCopy(renderer, textTexture, NULL, &textRect);
        SDL_DestroyTexture(textTexture);

        total_text_height += text_height + 5;
        line = strtok(NULL, "\n");
        line_count++;
    }

    // Petit rectangle contenant les informations de performance
    int small_rect_width = right_rect_width * 0.9;
    int small_rect_height = right_rect_height * 0.25;
    int small_rect_x = right_rect_x + (right_rect_width - small_rect_width) / 2;
    int small_rect_y = text_y + total_text_height + 20;

    SDL_Rect small_rect = { small_rect_x, small_rect_y, small_rect_width, small_rect_height };
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 50); // Blanc semi-transparent
    SDL_RenderFillRect(renderer, &small_rect);

    // Afficher les informations de performance
    char info_text[4][100];
    snprintf(info_text[0], sizeof(info_text[0]), "Number of Shots: %d", num_shots);
    snprintf(info_text[1], sizeof(info_text[1]), "Number of Hits: %d", num_hits);
    snprintf(info_text[2], sizeof(info_text[2]), "Duration: %.2f seconds", duration);
    snprintf(info_text[3], sizeof(info_text[3]), "Final Score: %d", final_score);

    // Charger une police pour les infos
    int info_font_size = 20;
    TTF_Font* info_font = TTF_OpenFont("medias/font/Sora.ttf", info_font_size);
    if (!info_font) {
        printf("Erreur de chargement de la police : %s\n", TTF_GetError());
        TTF_CloseFont(font);
        return;
    }

    SDL_Color infoTextColor = {0, 0, 0, 255}; // Couleur noire

    int info_line_count = 0;

    for (int i = 0; i < 4; i++) {
        SDL_Surface* textSurface = TTF_RenderText_Blended(info_font, info_text[i], infoTextColor);
        if (!textSurface) {
            printf("Erreur de rendu du texte : %s\n", TTF_GetError());
            TTF_CloseFont(info_font);
            TTF_CloseFont(font);
            return;
        }
        SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
        SDL_FreeSurface(textSurface);

        int text_width, text_height;
        SDL_QueryTexture(textTexture, NULL, NULL, &text_width, &text_height);

        SDL_Rect textRect = {
            small_rect_x + (small_rect_width - text_width) / 2,
            small_rect_y + info_line_count * (text_height + 5) + 10,
            text_width,
            text_height
        };

        SDL_RenderCopy(renderer, textTexture, NULL, &textRect);
        SDL_DestroyTexture(textTexture);

        info_line_count++;
    }

    TTF_CloseFont(info_font); // Fermer la police des infos

    // Créer les boutons en utilisant CreateClickableElement
    int button_width = 200; // Boutons plus grands
    int button_height = 100;

    int buttons_y = small_rect_y + small_rect_height + 20;

    // Vérifier que les boutons ne sortent pas du rectangle principal
    if (buttons_y + button_height > right_rect_y + right_rect_height) {
        buttons_y = right_rect_y + right_rect_height - button_height - 10;
    }

    int button1_x = right_rect_x + right_rect_width * 0.25 - button_width / 2;
    int button2_x = right_rect_x + right_rect_width * 0.75 - button_width / 2;

    int w1 = button_width, h1 = button_height;
    int w2 = button_width, h2 = button_height;

    // Créer les boutons (définissez les fonctions ReplayGame et GoToHomeScreen)
    SDL_Color buttonTextColor = {255, 255, 255, 255}; // Couleur du texte des boutons
    int button_font_size = 20;

    CreateClickableElement(renderer, button1_x, buttons_y, &w1, &h1, "Replay", buttonTextColor, "medias/images/btn-play.png", ReplayGame, 10);
    CreateClickableElement(renderer, button2_x, buttons_y, &w2, &h2, "Home", buttonTextColor, "medias/images/btn-play.png", GoToHomeScreen, 10);

    // Nettoyer
    TTF_CloseFont(font);
    if (left_image) SDL_DestroyTexture(left_image);

    // Présenter le rendu
    SDL_RenderPresent(renderer);

    // Boucle d'événements pour attendre l'action de l'utilisateur
    SDL_Event event;
    int waiting = 1;
    while (waiting) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                waiting = 0;
            }
            // Gérer les événements pour les boutons
            else if (event.type == SDL_MOUSEBUTTONDOWN) {
                int x = event.button.x;
                int y = event.button.y;
                // Vérifier si le clic est sur le bouton "Replay"
                if (x >= button1_x && x <= button1_x + w1 && y >= buttons_y && y <= buttons_y + h1) {
                    // Appeler la fonction de rappel pour rejouer
                    ReplayGame(&event);
                    waiting = 0;
                }
                // Vérifier si le clic est sur le bouton "Home"
                else if (x >= button2_x && x <= button2_x + w2 && y >= buttons_y && y <= buttons_y + h2) {
                    // Appeler la fonction de rappel pour retourner à l'écran d'accueil
                    GoToHomeScreen(&event);
                    waiting = 0;
                }
            }
        }
        SDL_Delay(100); // Petit délai pour éviter de consommer trop de ressources
    }
}

void DrawHints(Grid* grid, int hint[10][10]) {
    // Limiter les itérations à la taille minimale entre la grille et la matrice de hints
    int maxWidth = (grid->gridWidth < 10) ? grid->gridWidth : 10;
    int maxHeight = (grid->gridHeight < 10) ? grid->gridHeight : 10;

    // Définir le rayon des cercles (ajustable selon la taille des cellules)
    int radius = grid->cellSize / 4;

    for (int j = 0; j < maxHeight; j++) { // Itération sur les lignes (axe Y)
        for (int i = 0; i < maxWidth; i++) { // Itération sur les colonnes (axe X)
            // Calculer le centre de la cellule
            int centerX = grid->xPos + i * grid->cellSize + grid->cellSize / 2;
            int centerY = grid->yPos + j * grid->cellSize + grid->cellSize / 2;

            // Déterminer la couleur en fonction de l'état du hint
            if (hint[j][i] == 1) { // Tiré sur une cellule vide
                SDL_SetRenderDrawColor(grid->renderer, 255, 0, 0, 255);  // Rouge
            }
            else if (hint[j][i] == 2) { // Touché un bateau
                SDL_SetRenderDrawColor(grid->renderer, 0, 255, 0, 255);  // Vert
            }
            else if (hint[j][i] == 3) { // Bateau coulé
                SDL_SetRenderDrawColor(grid->renderer, 0, 0, 0, 255);    // Noir
            }
            else { // État non défini ou autre (aucun tir)
                continue; // Ne rien dessiner pour les cellules non touchées
            }

            // Dessiner le cercle rempli
            DrawFilledCircle(grid->renderer, centerX, centerY, radius);
        }
    }

    // Optionnel : Réinitialiser la couleur après le dessin
    SDL_SetRenderDrawColor(grid->renderer, 255, 255, 255, 255); // Blanc par défaut
}
void FireAtCell(Player* adversary, int boat_pos[10][10], int hint[10][10], int col, int row)
{
    printf("Lancement de FireAtCell.\n");
    
    // Validation des coordonnées
    if (col < 0 || col >= 10 || row < 0 || row >= 10) {
        printf("Coordonnées (%d, %d) hors limites!\n", col, row);
        return;
    }

    printf("Tir dans la cellule (Ligne: %d, Colonne: %d)\n", row, col);

    // Vérifier si la cellule a déjà été tirée
    if (hint[row][col] != 0) {
        printf("Cellule déjà tirée\n");
        return; // Si déjà tirée, on ne fait rien
    }

    // Vérifier si un bateau se trouve dans la cellule ciblée
    if (boat_pos[row][col] != 0) {
        // Touché
        adversary->health -= 1;
        printf("Touché!\n");
        printf("Le joueur %s a maintenant %d point(s) de vie.\n", adversary->name, adversary->health);
        int boat_id = boat_pos[row][col];  // Identifier le bateau
        hint[row][col] = 2;  // Marquer comme touché dans la matrice hint

        // Vérifier si la totalité du bateau est coulée
        bool boat_sunk = true;

        // Parcourir l'ensemble de la grille pour vérifier les autres parties du bateau
        for (int r = 0; r < 10 && boat_sunk; r++) {
            for (int c = 0; c < 10; c++) {
                // Si une autre partie du bateau est encore intacte (non touchée), le bateau n'est pas coulé
                if (boat_pos[r][c] == boat_id && hint[r][c] != 2) {
                    boat_sunk = false;
                    break;
                }
            }
        }

        // Si le bateau est coulé, marquer toutes ses parties comme coulées dans hint
        if (boat_sunk) {
            printf("Bateau coulé!\n");
            for (int r = 0; r < 10; r++) {
                for (int c = 0; c < 10; c++) {
                    if (boat_pos[r][c] == boat_id) {
                        hint[r][c] = 3;  // Marquer comme coulé
                    }
                }
            }
        }

    } else {
        // Manqué
        printf("Manqué!\n");
        hint[row][col] = 1;  // Marquer comme manqué dans la matrice hint
    }

    // Afficher les matrices mises à jour
    printf("Grille des bateaux :\n");
    afficherMatrice(boat_pos);
    printf("Grille des tirs :\n");
    afficherMatrice(hint);
    DrawHints(&play_grid,hint_player_one_grid);
    DrawHints(&radar_grid,hint_player_two_grid);
    // Ajout du délai de 200 millisecondes après chaque exécution
    //SDL_Delay(400);
}
void FireCallback(){
    printf("fire on a cell");
    score_shots++;
    if(num_shots==4){
    // Obtenir les cellules sélectionnées de l'interface utilisateur faire une boucle pour shooter dans les cell
    for (int i = 0; i < num_shots; i++) {
        FireAtCell(&player_two,player_two_grid,hint_player_one_grid, selected_cells[i][0], selected_cells[i][1]);
    }
    // Réinitialiser les cellules sélectionnées
    num_shots = 0;
    //et les selected cells
    for (int i = 0; i < MAX_SHOTS; i++) {
        selected_cells[i][0] = -1;
        selected_cells[i][1] = -1;
    }
    //change who_Played
    //*who_Played=2;
    //faire jouer le joueur 2
    printf("player 2 plying:");
    player_two_acting();
    }
    else{
        printf("you have to select 4 cells");
    }
}
void PlayingInterface(SDL_Window* Window, SDL_Renderer* Renderer) {
    time_t start_time, end_time;
    start_time = time(NULL);
    SDL_SetRenderDrawColor(Renderer, 0, 0, 0, 255);
    //SDL_RenderClear(Renderer);
      int width = 0; 
    int height = 0; 
    SDL_Color textColor = {255, 255, 255, 255};
    // Charger et afficher l'image de fond
    SDL_Surface *backgroundSurface = IMG_Load("medias/images/metalic_panel.png");
    if (!backgroundSurface) {
        return;
    }
    SDL_Texture *backgroundTexture = SDL_CreateTextureFromSurface(Renderer, backgroundSurface);
    SDL_FreeSurface(backgroundSurface);

    SDL_RenderCopy(Renderer, backgroundTexture, NULL, NULL);

    Fleet fleet = player_one.fleet;

    // Initialisation des grilles
    
    play_grid.renderer = Renderer;
    play_grid.gridWidth = 10;
    play_grid.gridHeight = 10;
    play_grid.cellSize = 41;
    play_grid.xPos = 50;
    play_grid.yPos = 90;

    
    radar_grid.renderer = Renderer;
    radar_grid.gridWidth = 10;
    radar_grid.gridHeight = 10;
    radar_grid.cellSize = 31;
    radar_grid.xPos = 530;
    radar_grid.yPos = 230;

    SDL_Surface* bgSurface = IMG_Load("medias/images/oceangrid.png");
    radar_grid.backgroundImage = SDL_CreateTextureFromSurface(Renderer, bgSurface);
    play_grid.backgroundImage = SDL_CreateTextureFromSurface(Renderer, bgSurface);
    SDL_FreeSurface(bgSurface);

    // Dessiner les grilles
    DrawGrid(&play_grid);
    DrawGrid(&radar_grid);
    // Ajouter les textures des bateaux
    add_texture_ship(&fleet.aircraft_carrier, Renderer, "medias/images/ships/cruiser-180.png", "medias/images/ships/cruiser-90.png");
    add_texture_ship(&fleet.cruiser, Renderer, "medias/images/ships/cruiser-180.png", "medias/images/ships/cruiser-90.png");
    add_texture_ship(&fleet.destroyer, Renderer, "medias/images/ships/cruiser-180.png", "medias/images/ships/cruiser-90.png");
    add_texture_ship(&fleet.submarine, Renderer, "medias/images/ships/cruiser-180.png", "medias/images/ships/cruiser-90.png");
    add_texture_ship(&fleet.torpedo_boat, Renderer, "medias/images/ships/cruiser-180.png", "medias/images/ships/cruiser-90.png");
    //Fire button 

    DrawFleet(&radar_grid, &fleet);
    // Charger l'image de "target"
    SDL_Surface *targetSurface = IMG_Load("medias/images/targeting.png");
    SDL_Texture *targetTexture = SDL_CreateTextureFromSurface(Renderer, targetSurface);
    SDL_FreeSurface(targetSurface);

    

    int quit = 0;
    SDL_Event event;
    int mouse_x, mouse_y;
    int cell_x = -1, cell_y = -1;
    int highlight_x = -1, highlight_y = -1;
    ClearEvents();
     CreateClickableElement(Renderer,200,500,&width,&height, "FIRE!!!", textColor,"medias/images/btn-play.png",FireCallback,12);
    //SDL_RenderPresent(Renderer);
    while (!quit && ((player_one.health!=0)&&(player_two.health!=0))) {
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    quit = 1;
                    break;

                case SDL_MOUSEMOTION:
                    SDL_GetMouseState(&mouse_x, &mouse_y);

                    // Si la souris est dans la zone de la grille play_grid
                    if (mouse_x >= play_grid.xPos && mouse_x < play_grid.xPos + play_grid.gridWidth * play_grid.cellSize &&
                        mouse_y >= play_grid.yPos && mouse_y < play_grid.yPos + play_grid.gridHeight * play_grid.cellSize) {
                        // Calculer la cellule de la grille
                        cell_x = (mouse_x - play_grid.xPos) / play_grid.cellSize;
                        cell_y = (mouse_y - play_grid.yPos) / play_grid.cellSize;
                        highlight_x = cell_x;
                        highlight_y = cell_y;
                    } else {
                        highlight_x = -1;
                        highlight_y = -1;
                    }
                    break;

                case SDL_MOUSEBUTTONDOWN:
                    printf("Click handle");
                    TriggerClickCallbacks(mouse_x,mouse_y);
                    printf("hightlight %d",highlight_y);
                    printf("hightlight %d",highlight_x);
                    if(event.button.button == SDL_BUTTON_RIGHT){
                        printf("right click");
                    }
                    if (highlight_x != -1 && highlight_y != -1) {
                        int already_selected = 0;
                        printf("selectionner cellule");
                        // Vérifier si cette cellule est déjà sélectionnée
                        for (int i = 0; i < num_shots; i++) {
                            if (selected_cells[i][0] == highlight_x && selected_cells[i][1] == highlight_y) {
                                already_selected = 1;

                                // Si déjà sélectionné, désélectionner
                                for (int j = i; j < num_shots - 1; j++) {
                                    selected_cells[j][0] = selected_cells[j + 1][0];
                                    selected_cells[j][1] = selected_cells[j + 1][1];
                                }
                                selected_cells[num_shots - 1][0] = -1;
                                selected_cells[num_shots - 1][1] = -1;
                                num_shots--;
                                break;
                            }
                        }

                        // Si non sélectionné et qu'il reste des tirs, ajouter la cellule
                        if (!already_selected && num_shots < MAX_SHOTS) {
                            printf("selected!!");
                            selected_cells[num_shots][0] = highlight_x;
                            selected_cells[num_shots][1] = highlight_y;
                            num_shots++;
                        }
                    }
                    break;
            }
        }

        // Redessiner l'interface
        //SDL_SetRenderDrawColor(Renderer, 0, 0, 0, 255);
        //SDL_RenderClear(Renderer);

        // Redessiner les éléments de la grille
        //SDL_RenderCopy(Renderer, backgroundTexture, NULL, NULL);
        DrawGrid(&play_grid);
        DrawGrid(&radar_grid);
        DrawFleet(&radar_grid,&fleet);
       // DrawFleet(&play_grid,&player_two.fleet);
        DrawHints(&play_grid,hint_player_one_grid);
        DrawHints(&radar_grid,hint_player_two_grid);
        // Dessiner la "target" qui suit la souris dans la grille
        if (highlight_x != -1 && highlight_y != -1) {
            SDL_Rect targetRect = {play_grid.xPos + highlight_x * play_grid.cellSize, 
                                   play_grid.yPos + highlight_y * play_grid.cellSize, 
                                   play_grid.cellSize, play_grid.cellSize};
            SDL_SetTextureAlphaMod(targetTexture, 128);  // Transparence
            SDL_RenderCopy(Renderer, targetTexture, NULL, &targetRect);
        }

        // Dessiner les cellules sélectionnées
        DrawSelectedCells(&play_grid, selected_cells, num_shots, targetTexture);

        SDL_RenderPresent(Renderer);  // Mettre à jour l'affichage
    }

    
    SDL_DestroyTexture(targetTexture);
    SDL_DestroyTexture(backgroundTexture);
    end_time = time(NULL);
    double duration = difftime(end_time, start_time);
    int failed_shots=score_shots-20;//failed shots 
    //calculate score with the duration the number of shoots"score_shots" and the number of failed shoot
    int final_score = (int)(1000 / (duration + 1) * (score_shots - 20)); 
    //fin de jeux
 if (player_one.health == 0) {
    // Le joueur un a perdu
    DrawWinWidget(Window, Renderer, "lose",player_two.name,score_shots,20,duration,final_score);
} else if (player_two.health == 0) {
    // Le joueur un a gagné
    DrawWinWidget(Window, Renderer, "win",player_one.name,score_shots,20,duration,final_score);
}


}
