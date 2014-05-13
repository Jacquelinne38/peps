using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using DALAssetDotNet;


namespace DataManagement
{
    class DataAssetValuesLoader
    {

        private bool _debug = false;
        private DateTime _begin;
        public System.DateTime Begin
        {
            get { return _begin; }
            set { _begin = value; }
        }
        private DateTime _end;
        public System.DateTime End
        {
            get { return _end; }
            set { _end = value; }
        }
        private double?[,] _matrix;
        public double[,] Matrix
        {
            get {
                    double[,] tempArray = new double[_lstassetName.Count, _lstassetDate.Count];
                    for (int i = 0; i < _lstassetName.Count; i++)
                    {
                        for (int j = 0; j < _lstassetDate.Count; j++)
                        {
                            tempArray[i, j] = (double)_matrix[i, j];

                        }
                    }
                    return tempArray;
            }
        }
        private List<String> _lstassetName;
        public List<String> LstassetName
        {
            get { return _lstassetName; }
            set { _lstassetName = value; }
        }
        private List<DateTime?> _lstassetDate;
        public List<DateTime?> LstassetDate
        {
            get { return _lstassetDate; }
            set { _lstassetDate = value; }
        }
        public DataAssetValuesLoader(DateTime beginImport, DateTime endImport, bool isDebug = false) {
            this._begin = beginImport;
            this._end = endImport;
            this._debug = isDebug;
            this._lstassetDate = new List<DateTime?>();
            this._lstassetName = new List<String>();
            ImportData();
        }

        private void ImportData() {
            List<String> assetsName = DALAssetDotNet.DALAssetsInfo.getDistinctAssetNameGO();
            var assetsValues = new List<List<AssetsValue>>();
            foreach (String name in assetsName)
            {
                List<AssetsValue> lstAssetsValues = DALAssetDotNet.DALAssetsValue.getValueAssetFrom(name, _begin, _end);
                assetsValues.Add(lstAssetsValues);
            }
            this._matrix = NormalizeData(assetsValues);
        }


        private double?[,] NormalizeData(List<List<AssetsValue>> lst)
        {
            double?[,] matrixValues = GetPartialAssetDateMatrix(lst);
            //debug
            if(_debug)
                Utils.SaveMatrix(matrixValues, lst.Count, matrixValues.Length / lst.Count, @"C:\Users\pizzix-game\Documents\GitHub\peps\peps\DATA\partiel.txt");
            matrixValues = SetNullValue(matrixValues, lst.Count, matrixValues.Length/lst.Count);

            //debug
            if (_debug)
                Utils.SaveMatrix(matrixValues, lst.Count, matrixValues.Length / lst.Count, @"C:\Users\pizzix-game\Documents\GitHub\peps\peps\DATA\complet.txt");
            return matrixValues;
        }

        /*
         * Change les valeurs -1 en suivante/précedente
         */
        private double?[,] SetNullValue(double?[,] matrix, int nbActif, int nbDate) 
        {
            int cout = 0;//debug
            for(int i = 0 ; i < nbActif; i++) {
                for(int j = 0; j < nbDate; j++) {
                    if (matrix[i, j] == -1)
                    {
                        //cas en 0
                        if (j == 0)
                        {
                            int it = 1;
                            for (; matrix[i, j + it] == -1; it++) ;
                            matrix[i, j] = matrix[i, j + it];
                        }
                        //cas entre 1 et max
                        else if (j == nbDate -1) {
                            int it = -1;
                            for (; matrix[i, j + it] == -1; --it) ;
                            matrix[i, j] = matrix[i, j + it];
                        }
                        else
                        {
                            int it = -1;
                            for (; matrix[i, j + it] == -1; --it) ;
                            matrix[i, j] = matrix[i, j + it];
                        }
                        cout++;
                    }
                }
            }
            return matrix;
        }

        /*
         * 
         * Renvoi la plus grande liste de date que l'on peut avoir sur l'échantillons
         */
        private List<DateTime?> GetLstMaxRange(List<List<AssetsValue>> lst) 
        {
            List<DateTime?> lstDate = new List<DateTime?>();
            foreach (AssetsValue a in lst.OrderByDescending(x => x.Count).First().OrderBy(x => x.Date))
            {
                lstDate.Add(a.Date);
            }
            return lstDate;
        }

        /*
         * Renvoi une matrice line = asset colonne = date
         * Pour chaque valeur manquante on écris -1
         */
        private double?[,] GetPartialAssetDateMatrix(List<List<AssetsValue>> lst)
        {
            _lstassetDate = GetLstMaxRange(lst);
            double?[,] matrixValues = new double?[lst.Count, _lstassetDate.Count];
            int count = 0;//debug
            for (int i = 0; i < lst.Count; i++)
            {
                for (int j = 0; j < _lstassetDate.Count; j++)
                {
                    try
                    {
                        var value = lst.OrderByDescending(x => x.Count).ToArray()[i].Where(x => x.Date == _lstassetDate[j]).First();
                        //TODO ici on doit créer une liste qui insert le nom de lactif comme ça on pourra savoir dans quel ordre sont les actif dans la matrice
                        //en faite c'est lst
                        //penser a faire pareil pour les tableaux de date
                        matrixValues[i, j] = value.close;
                    }
                    catch (Exception e)
                    {
                        matrixValues[i, j] = -1;
                        count++;
                    }
                }
                _lstassetName.Add(lst.OrderByDescending(x => x.Count).ToArray()[i].First().namego);
            }
            return matrixValues;
        }
        }
}
