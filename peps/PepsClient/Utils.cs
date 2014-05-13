using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace DataManagement
{
    public static class Utils
    {
        public static void SaveMatrix(double?[,] mt, int nbActif, int nbDate, String filestr)
        {
            using (System.IO.StreamWriter file = new System.IO.StreamWriter(filestr, false))
            {
                for (int i = 0; i < nbActif; i++)
                {
                    String tmp = "";
                    for (int j = 0; j < nbDate; j++)
                    {
                        tmp += mt[i, j] + ";";
                    }
                    file.WriteLine(tmp);
                }
            }
        }
    }
}
