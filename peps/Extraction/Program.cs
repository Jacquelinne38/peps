using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using APIFiMag;
using APIFiMag.Datas;
using APIFiMag.Importer;
using APIFiMag.Exporter;

namespace Extraction
{
    class Program
    {
        static void Main(string[] args)
        {
            List<string> _symboles = new List<string>();
            _symboles.Add("NYSE:AA");
         //   _symboles.Add("AMS:MT");
            _symboles.Add("LON:BARC");
          //  _symboles.Add("TYO:4568");
            _symboles.Add("FRA:DTE");
            _symboles.Add("NYSE:DYN");
            _symboles.Add("NYSE:GE");
            _symboles.Add("NYSE:HOG");
            _symboles.Add("FRA:IFX");
            _symboles.Add("NYSE:IP");
            _symboles.Add("NYSE:JCP");
          //  _symboles.Add("AMS:DSM");
            _symboles.Add("NYSE:MHFI");
           // _symboles.Add("TYO:8058");
          //  _symboles.Add("TYO:7974");
          //  _symboles.Add("HEL:NOK1V");
          //  _symboles.Add("HKG:0857");
            _symboles.Add("NYSE:CTQ");
          //  _symboles.Add("TSE:BB");
            _symboles.Add("NYSE:TIF");
            
            /*_symboles.Add("AA");
            _symboles.Add("MT.AS");
            _symboles.Add("BARC.L");
            _symboles.Add("DB1.DE");
            _symboles.Add("DYN");
            _symboles.Add("GE");
            _symboles.Add("HOG");
            _symboles.Add("IFX.F");
            _symboles.Add("IP");
            _symboles.Add("JCP");
            _symboles.Add("DSM.AS");
            _symboles.Add("MHFI");
            _symboles.Add("NOK1V.MI");
            _symboles.Add("0857.HK");
            _symboles.Add("CTQ");
            _symboles.Add("BB.TO");
            _symboles.Add("TIF");

            */
            List<HistoricalColumn> _histoColumns = new List<HistoricalColumn>();
            _histoColumns.Add(HistoricalColumn.Open);

            DataActif dataAc = new DataActif(_symboles,
                                             _histoColumns,
                                             new DateTime(1990, 1, 2),
                                             DateTime.Now
                                             );
            dataAc.ImportData(new ImportGoogle());
            dataAc.Export(new ExportCSV(@"C:\\tmp\\export.csv"));

        }


    }
}
