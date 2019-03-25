#include <libgimp/gimp.h>

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

  /* Setting mandatory output values */
  *nreturn_vals = 1;
  *return_vals = values;

  values[0].type = GIMP_PDB_STATUS;
  values[0].data.d_status = status;

  /* Getting run_mode - we won't display a dialog if
   * we are in NONINTERACTIVE mode */
  run_mode = param[0].data.d_int32;

  bool enableRichPresence = true;

  if (run_mode != GIMP_RUN_NONINTERACTIVE)
  {
    if (enableRichPresence)
    {
      g_message("Enabling..\n");
      InitDiscord();
      g_message("Enabled!\n");
    }
    else
    {
      g_message("Disabling..\n");
      Discord_Shutdown();
      g_message("Disabled!\n");
    }
  }
}
