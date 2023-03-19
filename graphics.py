from diagrams import Diagram, Cluster, Edge
from diagrams.aws.compute import EC2
from diagrams.aws.database import RDS
from diagrams.aws.network import ELB

with Diagram("Major Flowchart", show=False, direction="TB"):
    ELB("lb") >> [EC2("course1"),
                  EC2("course2"),
                  EC2("course3"),
                  EC2("course4"),
                  EC2("course5")] >> RDS("events")