#!/usr/bin/env python3
from _BOP import BoolOperaPolygon, Polygon as myPolygon
import matplotlib.pyplot as plt
from matplotlib.patches import Polygon
import sys
import tkinter as tk
from matplotlib.figure import Figure
from matplotlib.backends.backend_tkagg import FigureCanvasTkAgg

def parse_input(input_file):
    polygons = []
    current_polygon = []
    operation = None

    with open(input_file, 'r') as file:
        content = file.read()
        lines = content.strip().split('\n')
        for line in lines:
            line = line.strip()
            if not line:
                continue  # Skip empty lines
            if line.startswith('Polygon'):
                if current_polygon:
                    polygons.append(current_polygon)
                    current_polygon = []
            elif line.startswith('Operation'):
                operation = line.split(' ')[1]
            else:
                points = line.strip('()').split('),(')
                contour = [(float(p.split(',')[0]), float(p.split(',')[1])) for p in points]
                current_polygon.append(contour)

    if current_polygon:
        polygons.append(current_polygon)

    return polygons, operation

def BOP_main(polygons, operation):
    p1 = myPolygon(polygons[0])
    p2 = myPolygon(polygons[1])
    # print(polygons[0])
    # print(polygons[1])
    # print(operation)
    # p1 = myPolygon([[(5, 7), (6, 4), (9, 4.5), (9.5, 8)]])
    # p2 = myPolygon([[(7.5, 8.1), (5, 5), (8, 4)]])
    # operation = "Union"
    BOP = BoolOperaPolygon([p1, p2], operation)
    BOP.create_SweepEvent()
    BOP.subdividing_edge()
    BOP.joining_edge()
    for i in range(len(BOP.resultP)):
        contour = BOP.resultP[i]
        for coor in contour:
            print(coor[0], coor[1])
        print(BOP.result_INOUT[i])
        print()

    return(BOP.resultP, BOP.result_INOUT)

# interactive mode
class PolygonDrawer:
    def __init__(self, root, mode):
        self.root = root
        self.fig = Figure(figsize=(7, 7))
        self.ax = self.fig.add_subplot(111)
        self.ax.grid(True)
        self.ax.set_xlim(0, 10)
        self.ax.set_ylim(0, 10)
        self.canvas = FigureCanvasTkAgg(self.fig, master=root)
        self.canvas.get_tk_widget().pack(side=tk.RIGHT, fill=tk.BOTH, expand=True, padx=10, pady=0)

        if mode==0:
            button_width = 10
            button_height = 3
            button_font = ("Helvetica", 15)
            frame1 = tk.Frame(root)
            frame1.pack(side=tk.TOP, pady=10)
            self.canvas.mpl_connect('button_press_event', self.on_click)
            self.bt1 = tk.Button(frame1, text="add\n contour", command=self.add_contour, width=button_width, height=button_height, font=button_font)
            self.bt1.pack(side=tk.TOP)
            self.bt2 = tk.Button(frame1, text="P1", command=self.add_polygon, width=button_width, height=button_height, font=button_font)
            self.bt2.pack(side=tk.TOP)
            frame2 = tk.Frame(root, pady=50)
            frame2.pack(side=tk.TOP)
            self.bt3 = tk.Button(frame2, text="Union", command=self.union, width=button_width, height=button_height, font=button_font, state=tk.DISABLED)
            self.bt3.pack(side=tk.TOP)
            self.bt4 = tk.Button(frame2, text="Intersection", command=self.intersection, width=button_width, height=button_height, font=button_font, state=tk.DISABLED)
            self.bt4.pack(side=tk.TOP)
            self.bt5 = tk.Button(frame2, text="XOR", command=self.xor, width=button_width, height=button_height, font=button_font, state=tk.DISABLED)
            self.bt5.pack(side=tk.TOP)
            self.bt6 = tk.Button(frame2, text="Difference", command=self.difference, width=button_width, height=button_height, font=button_font, state=tk.DISABLED)
            self.bt6.pack(side=tk.TOP)
            frame3 = tk.Frame(root)
            frame3.pack(side=tk.BOTTOM)
            self.bt7 = tk.Button(frame3, text="clear", command=self.restart, width=button_width, height=button_height, font=button_font)
            self.bt7.pack(side=tk.BOTTOM)
        else:
            input_file = sys.argv[1]
            polygons, operation = parse_input(input_file)
            print(polygons)
            self.resultP, self.result_INOUT = BOP_main(polygons, operation)
            self.print_output()

        self.polygon_list = []
        self.polygon = []
        self.contour = []
    
    def on_click(self, event):
        if event.button == 1:
            # Add a vertex to the current polygon
            self.contour.append((event.xdata, event.ydata))
            self.update_plot()
            # print(self.contour)

    def update_plot(self):
        self.ax.clear()
        self.ax.grid(True)
        self.ax.set_xlim(0, 10)
        self.ax.set_ylim(0, 10)

        for poly in self.polygon_list:
            for contour in poly:
                polygon_patch = Polygon(contour, closed=True, edgecolor='darkgray', fill = "")
                self.ax.add_patch(polygon_patch)
                for vertex in contour:
                    self.ax.plot(vertex[0], vertex[1], 'ro')

        for contour in self.polygon:
            polygon_patch = Polygon(contour, closed=True, edgecolor='black', fill = "")
            self.ax.add_patch(polygon_patch)
            for vertex in contour:
                self.ax.plot(vertex[0], vertex[1], 'ro')

        polygon_patch = Polygon(self.contour, closed=True, edgecolor='black', fill = "")
        self.ax.add_patch(polygon_patch)
        for vertex in self.contour:
            self.ax.plot(vertex[0], vertex[1], 'ro')

        self.ax.autoscale_view()
        self.ax.set_aspect('equal', 'box')

        self.canvas.draw()

    def print_output(self):
        self.ax.clear()
        self.ax.grid(True)
        self.ax.set_xlim(0, 10)
        self.ax.set_ylim(0, 10)
        for i in range(len(self.resultP)):
            contour = self.resultP[i]
            if self.result_INOUT[i]:
                color = 'aqua'
            else:
                color = 'white'

            polygon = Polygon(contour, closed=True, facecolor=color, edgecolor='black')
            polygon.set_linewidth(3.0)
            self.ax.add_patch(polygon)

            for coor in contour:
                self.ax.plot(coor[0], coor[1], 'ro')
        self.ax.autoscale_view()
        self.ax.set_aspect('equal', 'box')

        self.canvas.draw()
        print("done")

    def add_contour(self):
        self.polygon.append(self.contour)
        self.contour = []

    def add_polygon(self):
        self.polygon_list.append(self.polygon)
        self.polygon = []
        if self.bt2['text'] == "P1":
            self.bt2.configure(text="P2")
        else:
            self.bt2.configure(state=tk.DISABLED)
            self.bt3.configure(state=tk.NORMAL)
            self.bt4.configure(state=tk.NORMAL)
            self.bt5.configure(state=tk.NORMAL)
            self.bt6.configure(state=tk.NORMAL)

    def union(self):
        print(self.polygon_list)
        self.resultP, self.result_INOUT = BOP_main(self.polygon_list, "Union")
        self.print_output()

    def intersection(self):
        print(self.polygon_list)
        self.resultP, self.result_INOUT = BOP_main(self.polygon_list, "Intersection")
        self.print_output()

    def xor(self):
        print(self.polygon_list)
        self.resultP, self.result_INOUT = BOP_main(self.polygon_list, "XOR")
        self.print_output()

    def difference(self):
        print(self.polygon_list)
        self.resultP, self.result_INOUT = BOP_main(self.polygon_list, "Difference")
        self.print_output()

    def restart(self):
        self.ax.clear()
        self.ax.grid(True)
        self.ax.set_xlim(0, 10)
        self.ax.set_ylim(0, 10)
        self.bt2.configure(state=tk.NORMAL)
        self.bt2.configure(text="P1")
        self.bt3.configure(state=tk.DISABLED)
        self.bt4.configure(state=tk.DISABLED)
        self.bt5.configure(state=tk.DISABLED)
        self.bt6.configure(state=tk.DISABLED)
        self.ax.autoscale_view()
        self.ax.set_aspect('equal', 'box')
        self.canvas.draw()
        self.polygon_list = []
        self.polygon = []
        self.contour = []


def interactive_main():
    root = tk.Tk()
    root.title("Polygon Drawer")
    if len(sys.argv) < 2:
        drawer = PolygonDrawer(root, 0)
    else:
        drawer = PolygonDrawer(root, 1)
    root.mainloop()

if __name__ == '__main__':
    interactive_main()