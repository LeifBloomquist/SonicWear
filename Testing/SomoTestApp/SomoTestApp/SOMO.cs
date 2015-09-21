using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace SomoTestApp
{
    class SOMO
    {
        public int UnitID { get; set; }

        public int AX { get; set; }
        public int AY { get; set; }
        public int AZ { get; set; }
        public int RX { get; set; }
        public int RY { get; set; }
        public int RZ { get; set; }
        public int MX { get; set; }
        public int MY { get; set; }
        public int MZ { get; set; }

        public long NumReceived { get; set; }

        public Stopwatch stopwatch { get; set; }

        public TimeSpan LastDelta { get; set; }
        public TimeSpan Average { get; set; }
    }
}
