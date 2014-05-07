using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using DALAssetDotNet;

namespace TestWrapper
{
    public static class DataResultEncoder
    {

        public static void ExportData(List<DateTime?> date, List<double> price, List<double> priceCouverture, List<double> sansRisque, List<double> risque) {
            DALResult.SaveResult(date, price, priceCouverture, sansRisque, risque);
        }

    
    }
}
