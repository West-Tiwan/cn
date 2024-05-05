#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/internet-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/applications-module.h"

using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("ARQExample");

int main (int argc, char *argv[])
{
  LogComponentEnableAll (LOG_LEVEL_INFO);

  NS_LOG_INFO ("Starting ARQ");

  CommandLine cmd;
  cmd.Parse (argc, argv);

  Time::SetResolution (Time::NS);

  NodeContainer nodes;
  nodes.Create (2);

  PointToPointHelper pointToPoint;
  pointToPoint.SetDeviceAttribute ("DataRate", StringValue ("5Mbps"));
  pointToPoint.SetChannelAttribute ("Delay", StringValue ("2ms"));

  NetDeviceContainer devices;
  devices = pointToPoint.Install (nodes);

  InternetStackHelper stack;
  stack.Install (nodes);

  Ipv4AddressHelper address;
  address.SetBase ("10.1.1.0", "255.255.255.0");
  Ipv4InterfaceContainer interfaces = address.Assign (devices);

  uint16_t port = 9;
  UdpServerHelper serverGbN (port);
  ApplicationContainer serverAppsGbN = serverGbN.Install (nodes.Get (1));
  serverAppsGbN.Start (Seconds (1.0));
  serverAppsGbN.Stop (Seconds (10.0));

  UdpClientHelper clientGbN (interfaces.GetAddress (1), port);
  clientGbN.SetAttribute ("MaxPackets", UintegerValue (1));
  clientGbN.SetAttribute ("Interval", TimeValue (Seconds (1.0)));
  clientGbN.SetAttribute ("PacketSize", UintegerValue (1024));
  ApplicationContainer clientAppsGbN = clientGbN.Install (nodes.Get (0));
  clientAppsGbN.Start (Seconds (2.0));
  clientAppsGbN.Stop (Seconds (10.0));

  Simulator::Run ();
  Simulator::Destroy ();

  NS_LOG_INFO ("ARQExample finished");

  return 0;
}
