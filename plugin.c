#include <gimp/libgimp/gimp.h>
#include <discord-rpc/include/discord_rpc.h>
#include <unistd.h>

static void query(void);
static void run(const gchar *name,
                gint nparams,
                const GimpParam *param,
                gint *nreturn_vals,
                GimpParam **return_vals);

GimpPlugInInfo PLUG_IN_INFO =
    {
        NULL,
        NULL,
        query,
        run};

MAIN()

void handleDiscordReady(const DiscordUser *connectedUser)
{
  printf("Discord ready!");
}

void handleDiscordError(int errcode, const char* message)
{
  printf("Discord error!");
}

void handleDiscordDisconnected(int errcode, const char* message)
{
  printf("Discord disconnected!");
}

void InitDiscord()
{
  DiscordEventHandlers handlers;
  memset(&handlers, 0, sizeof(handlers));
  handlers.ready = handleDiscordReady;
  handlers.errored = handleDiscordError;
  handlers.disconnected = handleDiscordDisconnected;

  // Discord_Initialize(const char* applicationId, DiscordEventHandlers* handlers, int autoRegister, const char* optionalSteamId, int pipe)
  Discord_Initialize("418562325121990661", &handlers, 1, NULL, 0);
}

typedef struct DiscordRichPresence {
    const char* state; /* max 128 bytes */
    const char* details; /* max 128 bytes */
    int64_t startTimestamp;
    int64_t endTimestamp;
    const char* largeImageKey; /* max 32 bytes */
    const char* largeImageText; /* max 128 bytes */
    const char* smallImageKey; /* max 32 bytes */
    const char* smallImageText; /* max 128 bytes */
    const char* partyId; /* max 128 bytes */
    int partySize;
    int partyMax;
    const char* matchSecret; /* max 128 bytes */
    const char* joinSecret; /* max 128 bytes */
    const char* spectateSecret; /* max 128 bytes */
    int8_t instance;
} DiscordRichPresence;

void UpdatePresence()
{
    gint* images_count = malloc(sizeof(gint));
    gint* images = gimp_image_list(images_count);
    gchar* uri = gimp_image_get_uri(images[0]);

    char buffer[256];
    DiscordRichPresence discordPresence;
    memset(&discordPresence, 0, sizeof(discordPresence));
    discordPresence.state = "GIMP";
    sprintf(buffer, "Editing %s", "debugging.gmp");
    discordPresence.details = buffer;
    discordPresence.endTimestamp = time(0) + 5 * 60;
    discordPresence.largeImageKey = "canary-large";
    discordPresence.smallImageKey = "ptb-small";
    discordPresence.instance = 1;
    Discord_UpdatePresence(&discordPresence);
}


static void query(void)
{
  static GimpParamDef args[] =
      {
          {GIMP_PDB_INT32,
           "run-mode",
           "Run mode"},
          {GIMP_PDB_IMAGE,
           "image",
           "Input image"},
          {GIMP_PDB_DRAWABLE,
           "drawable",
           "Input drawable"}};

  gimp_install_procedure(
      "gimp-rpc",
      "GIMP RPC",
      "Discord Rich Presence for GIMP",
      "Marc Rousavy",
      "Copyright Marc Rousavy",
      "2019",
      "_Toggle Discord Rich Presence...",
      "RGB*, GRAY*",
      GIMP_PLUGIN,
      G_N_ELEMENTS(args), 0,
      args, NULL);

  gimp_plugin_menu_register("gimp-rpc", "<Image>/Tools/Discord Rich Presence");
}

static void run(const gchar *name,
                gint nparams,
                const GimpParam *param,
                gint *nreturn_vals,
                GimpParam **return_vals)
{
  static GimpParam values[1];
  GimpPDBStatusType status = GIMP_PDB_SUCCESS;
  GimpRunMode run_mode;
  *nreturn_vals = 1;
  *return_vals = values;
  values[0].type = GIMP_PDB_STATUS;
  values[0].data.d_status = status;
  run_mode = param[0].data.d_int32;

  bool enableRichPresence = true;

  if (run_mode != GIMP_RUN_NONINTERACTIVE)
  {
    if (enableRichPresence)
    {
      g_message("Enabling..\n");
      InitDiscord();
      g_message("Enabled!\n");
      while(true) {
        print("Updating presence...");
        UpdatePresence();
        sleep(1000);
      }
    }
    else
    {
      g_message("Disabling..\n");
      Discord_Shutdown();
      g_message("Disabled!\n");
    }
  }
}
