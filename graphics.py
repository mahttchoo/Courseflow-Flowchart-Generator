# from diagrams import Diagram, Cluster, Edge
# from diagrams.aws.compute import EC2
# from diagrams.aws.database import RDS
# from diagrams.aws.network import ELB
# from diagrams.generic.blank import Blank
from graphviz import Digraph

# with Diagram("Major Flowchart", show=False, direction="TB"):
#     Blank("lb") >> [Blank("CSC 3000 (1)"),
#                   Blank("course2"),
#                   Blank("course3"),
#                   Blank("course4"),
#                   Blank("course5")]


graph = Digraph()
# graph.node("1", "CSC 3000 (1)")
# graph.node("2", "CSC 1230 (5)")
# graph.node("3", "CSC 2430 (5)")



graph.render('Flowchart.gv.pdf', view=True)

