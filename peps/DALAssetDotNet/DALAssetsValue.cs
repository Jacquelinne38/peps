using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
/*
 * 
 * 
 * 
 * */
namespace DALAssetDotNet
{
    public static class DALAssetsValue
    {
        //renvoi la valeur des actifs dans un range de date
      public static List<AssetsValue> getValueAssetFrom(string name, DateTime dateTime, DateTime dateTime_2)
        {
            DataPepsDataContext db = ContextDb.Connexion;

            var query = (from action in db.AssetsValue
                         where action.namego == name && action.Date >= dateTime && action.Date <= dateTime_2
                         orderby action.Date
                         select action);

            return query.ToList();
        }
    }
}
