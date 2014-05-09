using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;
using System.Web.Security;
using System.Web.SessionState;

namespace PepsClient
{
    public class Global : System.Web.HttpApplication
    {

        void Application_Start(object sender, EventArgs e)
        {
            // Code qui s'exécute au démarrage de l'application
            String _path = String.Concat(System.Environment.GetEnvironmentVariable("PATH"), ";", @"C:\Users\pizzix-game\Documents\GitHub\peps\peps\PepsClient\bin");

            
            System.Environment.SetEnvironmentVariable("PATH", _path, EnvironmentVariableTarget.Process);
            _path = "";
            _path = String.Concat(System.Environment.GetEnvironmentVariable("PATH"), ";", @"C:\Users\pizzix-game\Documents\GitHub\peps\peps\TestWrapper\obj\x86\Release");
            System.Environment.SetEnvironmentVariable("PATH", _path, EnvironmentVariableTarget.Process);
            _path = "";
            _path = String.Concat(System.Environment.GetEnvironmentVariable("PATH"), ";", @"C:\Users\pizzix-game\Documents\GitHub\peps\peps\Release");
            System.Environment.SetEnvironmentVariable("PATH", _path, EnvironmentVariableTarget.Process);
            _path = "";
            _path = String.Concat(System.Environment.GetEnvironmentVariable("PATH"), ";", @"C:\Users\pizzix-game\Documents\GitHub\peps\peps\Wrapper\Release");
            System.Environment.SetEnvironmentVariable("PATH", _path, EnvironmentVariableTarget.Process);
        }

        void Application_End(object sender, EventArgs e)
        {
            //  Code qui s'exécute à l'arrêt de l'application

        }

        void Application_Error(object sender, EventArgs e)
        {
            // Code qui s'exécute lorsqu'une erreur non gérée se produit

        }

        void Session_Start(object sender, EventArgs e)
        {
            // Code qui s'exécute lorsqu'une nouvelle session démarre

        }

        void Session_End(object sender, EventArgs e)
        {
            // Code qui s'exécute lorsqu'une session se termine. 
            // Remarque : l'événement Session_End est déclenché uniquement lorsque le mode sessionstate
            // a la valeur InProc dans le fichier Web.config. Si le mode de session a la valeur StateServer 
            // ou SQLServer, l'événement n'est pas déclenché.

        }

    }
}
