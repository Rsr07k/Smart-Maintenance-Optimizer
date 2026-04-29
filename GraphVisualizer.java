import javafx.application.Application;
import javafx.application.Platform;
import javafx.geometry.Insets;
import javafx.scene.Scene;
import javafx.scene.control.Button;
import javafx.scene.control.Label;
import javafx.scene.control.Separator;
import javafx.scene.control.TextArea;
import javafx.scene.layout.BorderPane;
import javafx.scene.layout.HBox;
import javafx.scene.layout.Pane;
import javafx.scene.layout.VBox;
import javafx.scene.paint.Color;
import javafx.scene.shape.Circle;
import javafx.scene.shape.Line;
import javafx.scene.text.Font;
import javafx.scene.text.FontWeight;
import javafx.scene.text.Text;
import javafx.stage.Stage;

import java.io.*;
import java.util.*;

public class GraphVisualizer extends Application {

    int N, E, budget;
    String[] types;
    int[][] edges;

    Set<String> dpSelected = new HashSet<>();
    Set<String> greedySelected = new HashSet<>();

    boolean[] isArticulation;
    Set<String> bridgeSet = new HashSet<>();

    static final double CENTER_X = 400;
    static final double CENTER_Y = 300;
    static final double RADIUS = 220;

    Line[] edgeLines;
    Circle[] nodeCircles;

    // status labels
    Label lblAlgorithm, lblCurrentEdge, lblAction;
    Label lblCurrentCost, lblCurrentPriority, lblRemainingBudget;
    Label lblSteps, lblTimeTaken, lblTimeComplexity;

    TextArea stepLog;

    boolean isRunning = false;

    public void start(Stage stage) throws Exception {
        readInputFile();
        readResultsFile();
        findArticulationAndBridges();

        BorderPane root = new BorderPane();
        root.setStyle("-fx-background-color: #1e1e2e;");

        Pane graphPane = buildGraphPane();
        VBox statusPanel = buildStatusPanel();
        HBox buttonBar = buildButtonBar();

        root.setCenter(graphPane);
        root.setRight(statusPanel);
        root.setBottom(buttonBar);

        stage.setTitle("Road Network Visualizer");
        stage.setScene(new Scene(root, 1100, 720));
        stage.show();
    }

    // ===================== GRAPH DRAWING =====================

    Pane buildGraphPane() {
        Pane pane = new Pane();
        pane.setStyle("-fx-background-color: #1e1e2e;");
        pane.setPrefSize(750, 650);

        double[] nodeX = new double[N];
        double[] nodeY = new double[N];
        edgeLines = new Line[E];
        nodeCircles = new Circle[N];

        for (int i = 0; i < N; i++) {
            double angle = 2 * Math.PI * i / N - Math.PI / 2;
            nodeX[i] = CENTER_X + RADIUS * Math.cos(angle);
            nodeY[i] = CENTER_Y + RADIUS * Math.sin(angle);
        }

        // draw edges
        for (int i = 0; i < E; i++) {
            int u = edges[i][0], v = edges[i][1];

            Line line = new Line(nodeX[u], nodeY[u], nodeX[v], nodeY[v]);
            line.setStroke(Color.GRAY);
            line.setStrokeWidth(2);
            edgeLines[i] = line;

            Text label = new Text((nodeX[u] + nodeX[v]) / 2 + 5,
                                  (nodeY[u] + nodeY[v]) / 2 - 5,
                                  "c=" + edges[i][2] + " p=" + edges[i][3]);
            label.setFill(Color.WHITE);
            label.setFont(Font.font("Arial", 11));

            pane.getChildren().addAll(line, label);
        }

        // draw nodes
        for (int i = 0; i < N; i++) {
            Circle c = new Circle(nodeX[i], nodeY[i], 25);
            String type = types[i].trim();

            if (isArticulation[i]) c.setFill(Color.RED);
            else if (type.equalsIgnoreCase("Important")) c.setFill(Color.YELLOW);
            else c.setFill(Color.LIGHTBLUE);

            nodeCircles[i] = c;

            Text id = new Text(nodeX[i] - 5, nodeY[i] + 5, String.valueOf(i));
            id.setFill(Color.BLACK);
            id.setFont(Font.font("Arial", FontWeight.BOLD, 14));

            Text typeLabel = new Text(nodeX[i] - 30, nodeY[i] + 45, type);
            typeLabel.setFill(Color.WHITE);
            typeLabel.setFont(Font.font(11));

            pane.getChildren().addAll(c, id, typeLabel);
        }

        drawLegend(pane);
        return pane;
    }

    // ===================== STATUS PANEL =====================

    VBox buildStatusPanel() {
        VBox panel = new VBox(8);
        panel.setPadding(new Insets(15));
        panel.setPrefWidth(320);
        panel.setStyle("-fx-background-color: #2a2a3d; -fx-border-color: #444; -fx-border-width: 0 0 0 2;");

        Label title = new Label("Status Panel");
        title.setFont(Font.font("Arial", FontWeight.BOLD, 16));
        title.setTextFill(Color.WHITE);

        lblAlgorithm       = makeLabel("Algorithm: -");
        lblCurrentEdge     = makeLabel("Current Edge: -");
        lblAction          = makeLabel("Action: -");
        lblCurrentCost     = makeLabel("Current Cost: 0");
        lblCurrentPriority = makeLabel("Current Priority: 0");
        lblRemainingBudget = makeLabel("Remaining Budget: " + budget);
        lblSteps           = makeLabel("Steps: 0");
        lblTimeTaken       = makeLabel("Time Taken: -");
        lblTimeComplexity  = makeLabel("Time Complexity: -");

        Label logTitle = new Label("Step History");
        logTitle.setFont(Font.font("Arial", FontWeight.BOLD, 13));
        logTitle.setTextFill(Color.WHITE);

        stepLog = new TextArea();
        stepLog.setEditable(false);
        stepLog.setPrefHeight(200);
        stepLog.setWrapText(true);
        stepLog.setStyle("-fx-control-inner-background: #1e1e2e; -fx-text-fill: #cccccc; "
                       + "-fx-font-family: 'Consolas'; -fx-font-size: 12;");

        panel.getChildren().addAll(
            title, new Separator(),
            lblAlgorithm, lblCurrentEdge, lblAction, new Separator(),
            lblCurrentCost, lblCurrentPriority, lblRemainingBudget, new Separator(),
            lblSteps, lblTimeTaken, lblTimeComplexity, new Separator(),
            logTitle, stepLog
        );
        return panel;
    }

    Label makeLabel(String text) {
        Label l = new Label(text);
        l.setTextFill(Color.LIGHTGRAY);
        l.setFont(Font.font("Arial", 13));
        l.setWrapText(true);
        return l;
    }

    // ===================== BUTTONS =====================

    HBox buildButtonBar() {
        HBox bar = new HBox(15);
        bar.setPadding(new Insets(10, 20, 10, 20));
        bar.setStyle("-fx-background-color: #2a2a3d;");

        String btnStyle = "-fx-background-color: #3a3a5c; -fx-text-fill: white; "
                        + "-fx-padding: 8 18; -fx-font-size: 13; -fx-cursor: hand;";

        Button dpBtn      = new Button("DP (Step-by-Step)");
        Button greedyBtn  = new Button("Greedy (Step-by-Step)");
        Button compareBtn = new Button("Compare");
        Button resetBtn   = new Button("Reset");

        dpBtn.setStyle(btnStyle);
        greedyBtn.setStyle(btnStyle);
        compareBtn.setStyle(btnStyle);
        resetBtn.setStyle("-fx-background-color: #5c3a3a; -fx-text-fill: white; "
                        + "-fx-padding: 8 18; -fx-font-size: 13; -fx-cursor: hand;");

        greedyBtn.setOnAction(e -> {
            if (isRunning) return;
            resetVisuals();
            runGreedyStepByStep();
        });

        dpBtn.setOnAction(e -> {
            if (isRunning) return;
            resetVisuals();
            runDPStepByStep();
        });

        compareBtn.setOnAction(e -> {
            if (isRunning) return;
            resetVisuals();
            showCompare();
        });

        resetBtn.setOnAction(e -> {
            isRunning = false;
            resetVisuals();
        });

        bar.getChildren().addAll(dpBtn, greedyBtn, compareBtn, resetBtn);
        return bar;
    }

    // ===================== GREEDY STEP-BY-STEP =====================

    void runGreedyStepByStep() {
        isRunning = true;
        lblAlgorithm.setText("Algorithm: Greedy");
        lblTimeTaken.setText("Time Taken: calculating...");
        lblTimeComplexity.setText("Time Complexity: calculating...");

        // sort edges by priority/cost ratio (same logic as C++)
        int[] order = new int[E];
        for (int i = 0; i < E; i++) order[i] = i;

        for (int i = 0; i < E - 1; i++) {
            for (int j = 0; j < E - 1 - i; j++) {
                double ratioA = edges[order[j]][2] == 0 ? 1e9
                              : (double) edges[order[j]][3] / edges[order[j]][2];
                double ratioB = edges[order[j+1]][2] == 0 ? 1e9
                              : (double) edges[order[j+1]][3] / edges[order[j+1]][2];
                if (ratioA < ratioB) {
                    int tmp = order[j];
                    order[j] = order[j+1];
                    order[j+1] = tmp;
                }
            }
        }

        long startTime = System.nanoTime();

        new Thread(() -> {
            int cost = 0, priority = 0, remaining = budget, step = 0;

            for (int s = 0; s < E; s++) {
                if (!isRunning) return;

                int idx = order[s];
                int u = edges[idx][0], v = edges[idx][1];
                int edgeCost = edges[idx][2], edgePri = edges[idx][3];

                // STEP 1: highlight orange = checking
                final int stepNum = ++step;
                Platform.runLater(() -> {
                    edgeLines[idx].setStroke(Color.ORANGE);
                    edgeLines[idx].setStrokeWidth(4);
                    highlightNode(u, true);
                    highlightNode(v, true);
                    lblCurrentEdge.setText("Current Edge: " + u + " - " + v);
                    lblAction.setText("Action: Checking edge...");
                    lblSteps.setText("Steps: " + stepNum);
                    addLog("Step " + stepNum + ": Edge " + u + "-" + v + " → Checking...");
                });

                delay(600);
                if (!isRunning) return;

                // STEP 2: decide select or skip
                final int fc = cost, fp = priority, fr = remaining;

                if (edgeCost <= remaining) {
                    cost += edgeCost;
                    priority += edgePri;
                    remaining -= edgeCost;

                    final int newC = cost, newP = priority, newR = remaining;
                    Platform.runLater(() -> {
                        edgeLines[idx].setStroke(Color.DODGERBLUE);
                        edgeLines[idx].setStrokeWidth(5);
                        highlightNode(u, false);
                        highlightNode(v, false);
                        lblAction.setText("Action: SELECTED (within budget)");
                        addLog("         → SELECTED (cost=" + edgeCost + ", budget left=" + newR + ")");
                        lblCurrentCost.setText("Current Cost: " + newC);
                        lblCurrentPriority.setText("Current Priority: " + newP);
                        lblRemainingBudget.setText("Remaining Budget: " + newR);
                    });
                } else {
                    Platform.runLater(() -> {
                        edgeLines[idx].setStroke(Color.web("#555555"));
                        edgeLines[idx].setStrokeWidth(1.5);
                        highlightNode(u, false);
                        highlightNode(v, false);
                        lblAction.setText("Action: SKIPPED (budget exceeded)");
                        addLog("         → SKIPPED (cost=" + edgeCost + " > budget=" + fr + ")");
                        lblCurrentCost.setText("Current Cost: " + fc);
                        lblCurrentPriority.setText("Current Priority: " + fp);
                        lblRemainingBudget.setText("Remaining Budget: " + fr);
                    });
                }

                delay(700);
            }

            // done
            long elapsed = (System.nanoTime() - startTime) / 1_000_000;
            final int totalC = cost, totalP = priority;
            Platform.runLater(() -> {
                lblAction.setText("Action: Completed");
                lblCurrentEdge.setText("Current Edge: -");
                lblTimeTaken.setText("Time Taken: " + elapsed + " ms");
                lblTimeComplexity.setText("Time Complexity: O(E log E)");
                addLog("--- Greedy Completed ---");
                addLog("Total Cost: " + totalC + "  Priority: " + totalP);
                isRunning = false;
            });
        }).start();
    }

    // ===================== DP STEP-BY-STEP =====================
    int hii(){
        return 0;
    }
    void runDPStepByStep() {
        int y = hii();
        isRunning = true;
        lblAlgorithm.setText("Algorithm: DP (Knapsack)");
        lblTimeTaken.setText("Time Taken: calculating...");
        lblTimeComplexity.setText("Time Complexity: calculating...");

        long startTime = System.nanoTime();
         
        new Thread(() -> {
            int cost = 0, priority = 0, remaining = budget, step = 0;

            for (int s = 0; s < E; s++) {
                if (!isRunning) return;

                int idx = s;
                int u = edges[idx][0], v = edges[idx][1];
                int edgeCost = edges[idx][2], edgePri = edges[idx][3];
                String key1 = u + "-" + v;
                String key2 = v + "-" + u;
                boolean isOptimal = dpSelected.contains(key1) || dpSelected.contains(key2);

                // STEP 1: highlight orange = evaluating
                final int stepNum = ++step;
                Platform.runLater(() -> {
                    edgeLines[idx].setStroke(Color.ORANGE);
                    edgeLines[idx].setStrokeWidth(4);
                    highlightNode(u, true);
                    highlightNode(v, true);
                    lblCurrentEdge.setText("Current Edge: " + u + " - " + v);
                    lblAction.setText("Action: Evaluating include/exclude...");
                    lblSteps.setText("Steps: " + stepNum);
                    addLog("Step " + stepNum + ": Edge " + u + "-" + v + " → Evaluating...");
                });

                delay(600);
                if (!isRunning) return;

                // STEP 2: decide include or exclude
                final int fc = cost, fp = priority, fr = remaining;

                if (isOptimal) {
                    cost += edgeCost;
                    priority += edgePri;
                    remaining -= edgeCost;

                    final int newC = cost, newP = priority, newR = remaining;
                    Platform.runLater(() -> {
                        edgeLines[idx].setStroke(Color.LIMEGREEN);
                        edgeLines[idx].setStrokeWidth(5);
                        highlightNode(u, false);
                        highlightNode(v, false);
                        lblAction.setText("Action: INCLUDED (optimal choice)");
                        addLog("         → INCLUDED (cost=" + edgeCost + ", budget left=" + newR + ")");
                        lblCurrentCost.setText("Current Cost: " + newC);
                        lblCurrentPriority.setText("Current Priority: " + newP);
                        lblRemainingBudget.setText("Remaining Budget: " + newR);
                    });
                } else {
                    Platform.runLater(() -> {
                        edgeLines[idx].setStroke(Color.web("#555555"));
                        edgeLines[idx].setStrokeWidth(1.5);
                        highlightNode(u, false);
                        highlightNode(v, false);
                        lblAction.setText("Action: EXCLUDED (not optimal)");
                        addLog("         → EXCLUDED");
                        lblCurrentCost.setText("Current Cost: " + fc);
                        lblCurrentPriority.setText("Current Priority: " + fp);
                        lblRemainingBudget.setText("Remaining Budget: " + fr);
                    });
                }

                delay(700);
            }

            // done
            long elapsed = (System.nanoTime() - startTime) / 1_000_000;
            final int totalC = cost, totalP = priority;
            Platform.runLater(() -> {
                lblAction.setText("Action: Completed");
                lblCurrentEdge.setText("Current Edge: -");
                lblTimeTaken.setText("Time Taken: " + elapsed + " ms");
                lblTimeComplexity.setText("Time Complexity: O(N x Budget)");
                addLog("--- DP Completed ---");
                addLog("Total Cost: " + totalC + "  Priority: " + totalP);
                isRunning = false;
            });
        }).start();
    }

    // ===================== COMPARE (instant) =====================

    void showCompare() {
        lblAlgorithm.setText("Algorithm: Compare (DP vs Greedy)");
        lblAction.setText("Action: Showing both selections");

        int dpCost = 0, dpPri = 0, grCost = 0, grPri = 0;

        for (int i = 0; i < E; i++) {
            int u = edges[i][0], v = edges[i][1];
            String k1 = u + "-" + v, k2 = v + "-" + u;

            boolean inDP     = dpSelected.contains(k1) || dpSelected.contains(k2);
            boolean inGreedy = greedySelected.contains(k1) || greedySelected.contains(k2);

            if (inDP && inGreedy) {
                edgeLines[i].setStroke(Color.PURPLE);
                edgeLines[i].setStrokeWidth(5);
            } else if (inDP) {
                edgeLines[i].setStroke(Color.LIMEGREEN);
                edgeLines[i].setStrokeWidth(5);
            } else if (inGreedy) {
                edgeLines[i].setStroke(Color.DODGERBLUE);
                edgeLines[i].setStrokeWidth(5);
            } else {
                edgeLines[i].setStroke(Color.GRAY);
                edgeLines[i].setStrokeWidth(2);
            }

            if (inDP)     { dpCost += edges[i][2]; dpPri += edges[i][3]; }
            if (inGreedy) { grCost += edges[i][2]; grPri += edges[i][3]; }
        }

        lblCurrentCost.setText("DP Cost: " + dpCost + " | Greedy Cost: " + grCost);
        lblCurrentPriority.setText("DP Priority: " + dpPri + " | Greedy Priority: " + grPri);
        lblRemainingBudget.setText("Budget: " + budget);
        lblCurrentEdge.setText("Current Edge: -");
        lblSteps.setText("Steps: -");
        lblTimeTaken.setText("Time Taken: -");
        lblTimeComplexity.setText("Time Complexity: -");

        if (dpPri > grPri)       lblAction.setText("Action: DP gives BETTER result!");
        else if (grPri > dpPri)  lblAction.setText("Action: Greedy gives BETTER result!");
        else                     lblAction.setText("Action: Both give SAME result!");
    }

    // ===================== HELPERS =====================

    void delay(int ms) {
        try { Thread.sleep(ms); } catch (Exception e) {}
    }

    void addLog(String msg) {
        stepLog.appendText(msg + "\n");
        stepLog.setScrollTop(Double.MAX_VALUE);
    }

    void highlightNode(int i, boolean on) {
        if (on) {
            nodeCircles[i].setStroke(Color.YELLOW);
            nodeCircles[i].setStrokeWidth(3);
            nodeCircles[i].setRadius(30);
        } else {
            nodeCircles[i].setStroke(null);
            nodeCircles[i].setRadius(25);
        }
    }

    void resetVisuals() {
        for (int i = 0; i < E; i++) {
            edgeLines[i].setStroke(Color.GRAY);
            edgeLines[i].setStrokeWidth(2);
            edgeLines[i].getStrokeDashArray().clear();
        }
        for (int i = 0; i < N; i++) {
            nodeCircles[i].setStroke(null);
            nodeCircles[i].setRadius(25);
        }

        lblAlgorithm.setText("Algorithm: -");
        lblCurrentEdge.setText("Current Edge: -");
        lblAction.setText("Action: -");
        lblCurrentCost.setText("Current Cost: 0");
        lblCurrentPriority.setText("Current Priority: 0");
        lblRemainingBudget.setText("Remaining Budget: " + budget);
        lblSteps.setText("Steps: 0");
        lblTimeTaken.setText("Time Taken: -");
        lblTimeComplexity.setText("Time Complexity: -");
        stepLog.clear();
    }

    // ===================== LEGEND =====================
   int john(){
    int a = 5, b= 10;
    return a+b;
   }
    void drawLegend(Pane root) {
        int x = 20, y = 20;

        Text title = new Text(x, y, "Legend");
        title.setFill(Color.WHITE);
        title.setFont(Font.font("Arial", FontWeight.BOLD, 13));

        Circle imp = new Circle(x, y + 20, 6, Color.YELLOW);
        Text t1 = new Text(x + 10, y + 25, "Important Node");
        t1.setFill(Color.LIGHTGRAY);

        Circle art = new Circle(x, y + 40, 6, Color.RED);
        Text t2 = new Text(x + 10, y + 45, "Articulation");
        t2.setFill(Color.LIGHTGRAY);

        Line dpL = new Line(x, y + 60, x + 20, y + 60);
        dpL.setStroke(Color.LIMEGREEN); dpL.setStrokeWidth(3);
        Text t3 = new Text(x + 25, y + 65, "DP Selected");
        t3.setFill(Color.LIGHTGRAY);

        Line grL = new Line(x, y + 80, x + 20, y + 80);
        grL.setStroke(Color.DODGERBLUE); grL.setStrokeWidth(3);
        Text t4 = new Text(x + 25, y + 85, "Greedy Selected");
        t4.setFill(Color.LIGHTGRAY);

        Line orL = new Line(x, y + 100, x + 20, y + 100);
        orL.setStroke(Color.ORANGE); orL.setStrokeWidth(3);
        Text t5 = new Text(x + 25, y + 105, "Checking");
        t5.setFill(Color.LIGHTGRAY);

        Line puL = new Line(x, y + 120, x + 20, y + 120);
        puL.setStroke(Color.PURPLE); puL.setStrokeWidth(3);
        Text t6 = new Text(x + 25, y + 125, "Both (Compare)");
        t6.setFill(Color.LIGHTGRAY);

        root.getChildren().addAll(title, imp, t1, art, t2, dpL, t3, grL, t4, orL, t5, puL, t6);
    }

    // ===================== FILE I/O =====================

    void readInputFile() throws Exception {
        BufferedReader br = new BufferedReader(new FileReader("input.txt"));

        budget = Integer.parseInt(br.readLine().trim());
        N = Integer.parseInt(br.readLine().trim());

        types = new String[N];
        for (int i = 0; i < N; i++) types[i] = br.readLine();

        E = Integer.parseInt(br.readLine().trim());
        edges = new int[E][6];

        for (int i = 0; i < E; i++) {
            String[] p = br.readLine().trim().split("\\s+");
            for (int j = 0; j < 6; j++)
                edges[i][j] = Integer.parseInt(p[j]);
        }
        int y = john();
        br.close();
    }

    void readResultsFile() {
        try {
            BufferedReader br = new BufferedReader(new FileReader("results.txt"));
            String line;

            while ((line = br.readLine()) != null) {
                line = line.trim();
                if (line.equals("DP")) {
                    int count = Integer.parseInt(br.readLine().trim());
                    for (int i = 0; i < count; i++) {
                        String[] p = br.readLine().trim().split("\\s+");
                        dpSelected.add(p[0] + "-" + p[1]);
                        dpSelected.add(p[1] + "-" + p[0]);
                    }
                }
                if (line.equals("GREEDY")) {
                    int count = Integer.parseInt(br.readLine().trim());
                    for (int i = 0; i < count; i++) {
                        String[] p = br.readLine().trim().split("\\s+");
                        greedySelected.add(p[0] + "-" + p[1]);
                        greedySelected.add(p[1] + "-" + p[0]);
                    }
                }
            }
            br.close();
        } catch (Exception e) {
            System.out.println("Run C++ first!");
        }
    }

    // ===================== ARTICULATION & BRIDGES =====================

    void findArticulationAndBridges() {
        isArticulation = new boolean[N];

        List<List<Integer>> adj = new ArrayList<>();
        for (int i = 0; i < N; i++) adj.add(new ArrayList<>());

        for (int i = 0; i < E; i++) {
            int u = edges[i][0], v = edges[i][1];
            adj.get(u).add(v);
            adj.get(v).add(u);
        }

        boolean[] visited = new boolean[N];
        int[] disc = new int[N], low = new int[N], parent = new int[N];
        Arrays.fill(parent, -1);

        for (int i = 0; i < N; i++)
            if (!visited[i])
                dfs(i, adj, visited, disc, low, parent, 0);
    }
   int arun2(){
    return -1;
   }
    void dfs(int u, List<List<Integer>> adj, boolean[] visited,
             int[] disc, int[] low, int[] parent, int time) {

        visited[u] = true;
        disc[u] = low[u] = ++time;
        int children = 0;
         int m = arun2();
        for (int v : adj.get(u)) {
            if (!visited[v]) {
                children++;
                parent[v] = u;
                dfs(v, adj, visited, disc, low, parent, time);
                low[u] = Math.min(low[u], low[v]);

                if (parent[u] == -1 && children > 1) isArticulation[u] = true;
                if (parent[u] != -1 && low[v] >= disc[u]) isArticulation[u] = true;
                if (low[v] > disc[u]) {
                    bridgeSet.add(u + "-" + v);
                    bridgeSet.add(v + "-" + u);
                }
            } else if (v != parent[u]) {
                low[u] = Math.min(low[u], disc[v]);
            }
        }
    }

    public static void main(String[] args) {
        launch();
    }
}