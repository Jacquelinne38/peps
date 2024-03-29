﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace DALAssetDotNet
{
    public static class  DALResult
    {
        //renvoi le dernier id utilisé
      public static int getLastRunId()
        {
            DataPepsDataContext db = ContextDb.Connexion;

            List<String> tmp = new List<string>();

            var result = db.Result.Max(x => x.idRun);
            if (result == null) result = 1;
            return result;
        }


        //sauvegarde les résultat d'un run
        public static void SaveResult(List<DateTime?> dates,
                                      List<double> prices,
                                      List<double> priceCouvertures,
                                      List<double> sansRisques,
                                      List<double> risques,
                                        double[,] compo  , 
            List<String> assets
            ) 
        {
           int runid = getLastRunId();
           List<Result> results = new List<Result>();
           for (int i = 0; i < prices.Count; ++i)
           {
               Result re = new Result();
               re.date = dates[i];
               re.idRun = runid+1;
               re.price = prices[i];
               re.priceCouverture = priceCouvertures[i];
               re.sansRisque = sansRisques[i];
               re.risque = risques[i];
               results.Add(re);
               // SaveOneResult(runid, dates[i], prices[i], priceCouvertures[i], sansRisques[i], risques[i]);
            }
            
            DataPepsDataContext db = ContextDb.Connexion;
            db.Result.InsertAllOnSubmit(results);
            db.SubmitChanges();

            //compo
            List<Composition> compos = new List<Composition>();
            for( int i = 0; i < compo.GetLength(0); i++) {
                for (int j = 0; j < prices.Count; j++)
                {
                    Composition tmp = new Composition();
                    tmp.idRun = runid +1;
                    tmp.actif = assets[i];
                    double val = compo[i, j];
                    if (double.IsNaN(val))
                        val = 0;
                    else if (double.IsInfinity(val))
                        val = 0;
                    else if (double.IsNegativeInfinity(val))
                        val = 0;
                    else if (double.IsPositiveInfinity(val))
                        val = 0;
                    tmp.value = val * 100;
                    tmp.date = dates[j];
                    compos.Add(tmp);
                }
            }

            db.Composition.InsertAllOnSubmit(compos);
            db.SubmitChanges();



            //db.SubmitChanges();
        }

        private static void SaveOneResult(int runId, DateTime? date, double price, double priceCouverture, double sansRisque, double risque) {
            DataPepsDataContext db = ContextDb.Connexion;
            Result re = new Result();
            re.date = date;
            re.idRun = runId;
            re.price = price;
            re.priceCouverture = priceCouverture;
            re.sansRisque = sansRisque;
            re.risque = risque;
            db.Result.InsertOnSubmit(re);
            db.SubmitChanges();
              

        }
    }
}
