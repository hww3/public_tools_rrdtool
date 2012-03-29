#!/usr/bin/env pike

int main(int argc, array argv) {
  string filename="loadavg.rrd";
  array DS=({
    "DS:load1:GAUGE:600:0:100",
    "DS:load5:GAUGE:600:0:100",
    "DS:load15:GAUGE:600:0:100"
  });
  array RRA=({
    "RRA:AVERAGE:0.5:1:576",
    "RRA:AVERAGE:0.5:6:672",
    "RRA:AVERAGE:0.5:24:732",
    "RRA:AVERAGE:0.5:144:1460"
  });

  int res=Public.Tools.RRDtool.rrdtool_create(filename,DS,RRA);
  write("res==%d\n",res);
  
}
