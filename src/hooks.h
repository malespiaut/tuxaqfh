
void update_hooks () ;
void hit_hook ( ssgEntity *ent, sgMat4 *last_mat, sgMat4 *curr_mat ) ;

void *invisibleInit ( ssgBranch **, char * ) ;
void *autodcsInit   ( ssgBranch **, char * ) ;
void *autotexInit   ( ssgBranch **, char * ) ;
void *tuxcopterInit ( ssgBranch **, char * ) ;
void *billboardInit ( ssgBranch **, char * ) ;
void *starwingInit  ( ssgBranch **, char * ) ;
void *switchOnInit  ( ssgBranch **, char * ) ;
void *switchOffInit ( ssgBranch **, char * ) ;
void *animInit      ( ssgBranch **, char * ) ;
void *snowballInit  ( ssgBranch **, char * ) ;
void *slamdcsInit   ( ssgBranch **, char * ) ;

void autodcsHook    ( ssgBranch *, void * ) ;
void animHook       ( ssgBranch *, void * ) ;
void tuxcopterHook  ( ssgBranch *, void * ) ;
void billboardHook  ( ssgBranch *, void * ) ;
void starwingHook   ( ssgBranch *, void * ) ;
void slamdcsHook    ( ssgBranch *, void * ) ;

void switchHit      ( ssgBranch *, void * ) ;
void snowballHit    ( ssgBranch *, void * ) ;

ssgEntity *getHandle ( int h ) ;

