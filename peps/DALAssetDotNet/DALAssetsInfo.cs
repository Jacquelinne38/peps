using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;



/*
 * 
 * Permet l'acces au produit à partir de la base de donnée
 * 
 * */
namespace DALAssetDotNet
{
    public static class DALAssetsInfo
    {
        //Nous renvoi la liste des sous jacent de notre produit
        public static List<String> getDistinctAssetNameGO()
        {
            DataPepsDataContext db = ContextDb.Connexion;

            List<String> tmp = new List<string>();

            var query = (
                
                from p in db.AssetsInfo
               // where p.currency != "JPY" & p.currency !="GBX" & p.currency != "HKD"
                
                         select p.namego)
                                        .Distinct();

            foreach (var item in query)
            {
                tmp.Add(item.ToString());
            }

            return tmp;
        }

        public static String[] getDistinctAssetNameGOArray()
        {
            List<String> ls = getDistinctAssetNameGO();
            return ls.ToArray();
        }

      
    }
}
