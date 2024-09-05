/********************************************************************************\
 **                                                                              **
 **  Copyright (C) 2024 Saif Kandil (k0T0z)                                      **
 **                                                                              **
 **  This file is a part of the ENIGMA Development Environment.                  **
 **                                                                              **
 **                                                                              **
 **  ENIGMA is free software: you can redistribute it and/or modify it under the **
 **  terms of the GNU General Public License as published by the Free Software   **
 **  Foundation, version 3 of the license or any later version.                  **
 **                                                                              **
 **  This application and its source code is distributed AS-IS, WITHOUT ANY      **
 **  WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS   **
 **  FOR A PARTICULAR PURPOSE. See the GNU General Public License for more       **
 **  details.                                                                    **
 **                                                                              **
 **  You should have recieved a copy of the GNU General Public License along     **
 **  with this code. If not, see <http://www.gnu.org/licenses/>                  **
 **                                                                              **
 **  ENIGMA is an environment designed to create games and other programs with a **
 **  high-level, fully compilable language. Developers of ENIGMA or anything     **
 **  associated with ENIGMA are in no way responsible for its users or           **
 **  applications created by its users, or damages caused by the environment     **
 **  or programs made in the environment.                                        **
 **                                                                              **
 \********************************************************************************/

#ifndef ENIGMA_VISUAL_SHADER_EDITOR_H
#define ENIGMA_VISUAL_SHADER_EDITOR_H

#include <QtCore/QJsonObject>
#include <QtCore/QPointF>
#include <QtCore/QSize>
#include <QtWidgets/QHBoxLayout>
#include <QPushButton>
#include <QDialog>
#include <QVBoxLayout>
#include <QTreeWidget>
#include <QTextEdit>

#include <string>
#include <vector>

#include <QtNodes/AbstractGraphModel>
#include <QtNodes/ConnectionIdUtils>
#include <QtNodes/StyleCollection>
#include <QtNodes/BasicGraphicsScene>
#include <QtNodes/GraphicsView>

#include "ResourceTransformations/VisualShader/visual_shader.h"
#include "BaseEditor.h"

using ConnectionId = QtNodes::ConnectionId;
using ConnectionPolicy = QtNodes::ConnectionPolicy;
using NodeFlag = QtNodes::NodeFlag;
using NodeId = QtNodes::NodeId;
using NodeRole = QtNodes::NodeRole;
using PortIndex = QtNodes::PortIndex;
using PortRole = QtNodes::PortRole;
using PortType = QtNodes::PortType;
using StyleCollection = QtNodes::StyleCollection;
using QtNodes::InvalidNodeId;

using QtNodes::BasicGraphicsScene;
using QtNodes::GraphicsView;

class VisualShaderGraph;
class CreateNodeDialog;

/*************************************/
/* VisualShaderEditor                */
/*************************************/

// Add const to any function that does not modify the object.

class VisualShaderEditor : public BaseEditor {
    Q_OBJECT

 public:
  VisualShaderEditor(MessageModel* model, QWidget* parent = nullptr);
  ~VisualShaderEditor() override;

  void create_node();

  void add_node();

  void show_create_node_dialog(const bool& custom_mouse_pos = false);

  std::vector<std::string> pasre_node_category_path(const std::string& node_category_path);
  QTreeWidgetItem* find_or_create_category_item(QTreeWidgetItem* parent, const std::string& category, const std::string& category_path, QTreeWidget* create_node_dialog_nodes_tree, std::unordered_map<std::string, QTreeWidgetItem*>& category_path_map);

 private:
  QHBoxLayout* layout;

  QHBoxLayout* scene_layer_layout;
  QWidget* scene_layer; // Layer having the scene.
  VisualShaderGraph* graph;
  BasicGraphicsScene* scene;
  GraphicsView* view;

  QWidget* top_layer; // Layer having the menu bar.
  QHBoxLayout* menu_bar;

  QPushButton* create_node_button;
  QPushButton* preview_shader_button;

  ////////////////////////////////////
  // CreateNodeDialog Nodes Tree
  ////////////////////////////////////

  struct CreateNodeDialogNodesTreeItem {
    std::string name;
    std::string category_path;
    std::string type;
    std::string description;
    std::vector<TVariant> ops;
    VisualShaderNode::PortType return_type;

    CreateNodeDialogNodesTreeItem(const std::string& name = std::string(), 
                                  const std::string& category_path = std::string(), 
                                  const std::string& type = std::string(), 
                                  const std::string& description = std::string(),
                                  const std::vector<TVariant>& ops = std::vector<TVariant>(),
                                  const VisualShaderNode::PortType& return_type = VisualShaderNode::PortType::PORT_TYPE_ENUM_SIZE) : name(name), 
                                                                                                                                     category_path(category_path), 
                                                                                                                                     type(type), 
                                                                                                                                     description(description),
                                                                                                                                     ops(ops),
                                                                                                                                     return_type(return_type) {}

  };

  static const VisualShaderEditor::CreateNodeDialogNodesTreeItem create_node_dialog_nodes_tree_items[];

  CreateNodeDialog* create_node_dialog;
};

/*************************************/
/* CreateNodeDialog                  */
/*************************************/

class CreateNodeDialog : public QDialog {
    Q_OBJECT

 public:
  CreateNodeDialog(QWidget* parent = nullptr);
  ~CreateNodeDialog();

  QTreeWidget* get_nodes_tree() const { return create_node_dialog_nodes_tree; }

 private slots:
  void on_CreateButtonTriggered();
  void on_CancelButtonTriggered();

 private:
  QVBoxLayout* layout;

  QVBoxLayout* create_node_dialog_nodes_tree_layout;

  QTreeWidget* create_node_dialog_nodes_tree;
  QTextEdit* create_node_dialog_nodes_description;

  QHBoxLayout* buttons_layout;
  QPushButton* create_button;
  QPushButton* cancel_button;
};

/*************************************/
/* VisualShaderGraph                 */
/*************************************/

/**
 * The class implements a bare minimum required to demonstrate a model-based
 * graph.
 */
class VisualShaderGraph : public QtNodes::AbstractGraphModel
{
    Q_OBJECT

public:
    struct NodeGeometryData
    {
        QSize size;
        QPointF pos;
    };

    VisualShaderGraph();

    ~VisualShaderGraph() override;

    std::unordered_set<NodeId> allNodeIds() const override;

    std::unordered_set<ConnectionId> allConnectionIds(NodeId const nodeId) const override;

    std::unordered_set<ConnectionId> connections(NodeId nodeId,
                                                 PortType portType,
                                                 PortIndex portIndex) const override;

    bool connectionExists(ConnectionId const connectionId) const override;

    NodeId addNode(QString const nodeType = QString()) override;

    /**
   * Connection is possible when graph contains no connectivity data
   * in both directions `Out -> In` and `In -> Out`.
   */
    bool connectionPossible(ConnectionId const connectionId) const override;

    void addConnection(ConnectionId const connectionId) override;

    bool nodeExists(NodeId const nodeId) const override;

    QVariant nodeData(NodeId nodeId, NodeRole role) const override;

    bool setNodeData(NodeId nodeId, NodeRole role, QVariant value) override;

    QVariant portData(NodeId nodeId,
                      PortType portType,
                      PortIndex portIndex,
                      PortRole role) const override;

    bool setPortData(NodeId nodeId,
                     PortType portType,
                     PortIndex portIndex,
                     QVariant const &value,
                     PortRole role = PortRole::Data) override;

    bool deleteConnection(ConnectionId const connectionId) override;

    bool deleteNode(NodeId const nodeId) override;

    QJsonObject saveNode(NodeId const) const override;

    /// @brief Creates a new node based on the informatoin in `nodeJson`.
    /**
   * @param nodeJson conains a `NodeId`, node's position, internal node
   * information.
   */
    void loadNode(QJsonObject const &nodeJson) override;

    void set_visual_shader_editor(VisualShaderEditor* visual_shader_editor) { this->visual_shader_editor = visual_shader_editor; }

private:
    std::unordered_set<NodeId> _nodeIds;

    /// [Important] This is a user defined data structure backing your model.
    /// In your case it could be anything else representing a graph, for example, a
    /// table. Or a collection of structs with pointers to each other. Or an
    /// abstract syntax tree, you name it.
    ///
    /// This data structure contains the graph connectivity information in both
    /// directions, i.e. from Node1 to Node2 and from Node2 to Node1.
    std::unordered_set<ConnectionId> _connectivity;

    mutable std::unordered_map<NodeId, NodeGeometryData> _nodeGeometryData;

    /// A convenience variable needed for generating unique node ids.
    NodeId _nextNodeId;

    NodeId newNodeId() override { return _nextNodeId++; }

    VisualShaderEditor* visual_shader_editor;
};

#endif // ENIGMA_VISUAL_SHADER_EDITOR_H
