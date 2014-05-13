using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using DALAssetDotNet;

namespace TestWrapper
{
    public static class DataResultEncoder
    {

        //Permet d'exporter les données après calcule.
        //Nos calcules sont asynchrones. Nous devons attendre la fin des calcules pour pouvoir les visualiser et les mettres en base de données.
        //Pour régler ce problème il aurait fallu créer une DAL pour le projet c++
        public static void ExportData(List<DateTime?> date, List<double> price, List<double> priceCouverture, List<double> sansRisque, List<double> risque) {
            DALResult.SaveResult(date, price, priceCouverture, sansRisque, risque);
        }

    
    }
}
