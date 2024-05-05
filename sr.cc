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

  NS_LOG_INFO ("Starting ARQExample");

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

  UdpServerHelper serverSR (port);
  ApplicationContainer serverAppsSR = serverSR.Install (nodes.Get (1));
  serverAppsSR.Start (Seconds (1.0));
  serverAppsSR.Stop (Seconds (10.0));

  UdpClientHelper clientSR (interfaces.GetAddress (1), port);
  clientSR.SetAttribute ("MaxPackets", UintegerValue (1));
  clientSR.SetAttribute ("Interval", TimeValue (Seconds (1.0)));
  clientSR.SetAttribute ("PacketSize", UintegerValue (1024));
  ApplicationContainer clientAppsSR = clientSR.Install (nodes.Get (0));
  clientAppsSR.Start (Seconds (2.0));
  clientAppsSR.Stop (Seconds (10.0));

  Simulator::Run ();
  Simulator::Destroy ();

  NS_LOG_INFO ("ARQExample finished");

  return 0;
}
