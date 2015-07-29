/*
 * Behavior_Tree.h
 *
 *  Created on: Mar 11, 2014
 *      Author: linyisheng
 */

#ifndef BEHAVIOR_TREE_H_
#define BEHAVIOR_TREE_H_

#include "AnyData.h"
#include <stdio.h>
#include <stdint.h>
#include <vector>

#define D_SafeDelete(p)							if(p) { delete p; p = NULL; }
#define k_BLimited_MaxChildNodeCnt              16
#define k_BLimited_InvalidChildNodeIndex        k_BLimited_MaxChildNodeCnt

typedef AnyData Bev_Node_Input_Param;
typedef AnyData Bev_Node_Output_Param;

enum Bev_Running_Status {
	k_BRS_Invalid					= -1,
	k_BRS_Continue                  = -2,

	k_BRS_Executing					= 0,       //true
	k_BRS_Finish					= 1,       //true
	k_BRS_SUCCESS                   = 1,       //true   成立
	k_BRS_Failure					= 2,       //false  前提不成立

	k_BRS_ERROR_Transition			= -1,
};

enum E_TerminalNodeStaus {
	k_TNS_Ready         = 1,
	k_TNS_Running       = 2,
	k_TNS_Finish        = 3,
};


class NPC;

struct Bev_Input_Data {
	double	time_step;
	int64_t npc_id;
	NPC*	npc;
};

struct Bev_Output_Data {
	double	m_TimeStep;
	NPC*	npc;
};

class AI_Node;
class AI_Task;

class AI_Node {
public:
	AI_Node() :m_pParent(NULL) {}
	virtual ~AI_Node(){};
	virtual AI_Task* create_task() = 0;
	virtual void destroy_task(AI_Task* pTask) = 0;

	void set_parent(AI_Node* node) {
		if(m_pParent) {
			//D_Output("has parent already?\n");
		}
		m_pParent = node;
	}

protected:
	AI_Node* m_pParent;
};
typedef std::vector<AI_Node*> AI_Nodes;


class AI_Task {
public:
	AI_Task(AI_Node* pNode) :m_pNode(pNode) {}
	virtual ~AI_Task(){};
	virtual void			 on_init(NPC *npc) = 0;
	virtual Bev_Running_Status on_update(NPC *npc) = 0;
	virtual void			 on_terminate(NPC *npc) = 0;

protected:
	AI_Node* m_pNode;
};

class Behavior {
public:
	Behavior() : m_eStatus(k_BRS_Invalid) {
		m_pTask = NULL;
		m_pNode = NULL;
	}

	Behavior(AI_Node& node)
		:m_pTask(NULL)
		,m_eStatus(k_BRS_Invalid) {
		install(node);
	}

	virtual ~Behavior() {
		uninstall();
	}

	bool has_installed() const {
		return m_pTask != NULL;
	}

	void install(AI_Node &node) {
		uninstall();

		m_pNode = &node;
		m_pTask = m_pNode->create_task();
	}

	void uninstall() {
		if(!m_pTask) return;
		if(!m_pNode) return;
		m_pNode->destroy_task(m_pTask);
		m_pTask = NULL;
		m_eStatus = k_BRS_Invalid;
	}

	Bev_Running_Status update(NPC *npc) {
		m_pTask->on_init(npc);
		Bev_Running_Status ret = m_pTask->on_update(npc);
		m_pTask->on_terminate(npc);

		return ret;
	}

private:
	AI_Node* m_pNode;
	AI_Task* m_pTask;
	Bev_Running_Status m_eStatus;
};


class Composite_Node : public AI_Node {
public:
	virtual ~Composite_Node() {
		for(size_t i = 0; i < m_Children.size(); ++i) {
			D_SafeDelete(m_Children[i]);
		}
	}

	AI_Node* get_child(int idx){
		if(idx < 0 || idx >= (int)m_Children.size())
			return NULL;
		return m_Children[idx];
	}

	void add_child(AI_Node* node) {
		node->set_parent(this);
		m_Children.push_back(node);
	}

	int get_child_count() const{
		return m_Children.size();
	}

protected:
	AI_Nodes m_Children;
};

/// 选择节点：有一个返回true,全部返回ture,反之返回false(每次都是从第一个子节点开始寻找可运行的节点，当找到后，就中断寻找的过程)
class Selector_Task : public AI_Task {
public:
	Selector_Task(AI_Node* pNode)
		: AI_Task(pNode)
		, m_LastBehavior(-1) {}

	Composite_Node& get_composite_node(){
		return *dynamic_cast<Composite_Node*>(m_pNode);
	}

	virtual void on_init(NPC *npc) {}

	virtual Bev_Running_Status on_update(NPC *npc) {
		Composite_Node& comNode = get_composite_node();
		if(comNode.get_child_count() == 0)
			return k_BRS_Failure;

		if(!m_CurrentBehavior.has_installed()) {
			m_LastBehavior = 0;
			m_CurrentBehavior.install(*(comNode.get_child(m_LastBehavior)));
		}
		Bev_Running_Status status = m_CurrentBehavior.update(npc);
		if(status != k_BRS_Failure) {
			return status;
		}
		for(int i = 0; i < comNode.get_child_count(); ++i) {
			if(m_LastBehavior == i)
				continue;

			m_CurrentBehavior.install(*(comNode.get_child(i)));
			Bev_Running_Status status = m_CurrentBehavior.update(npc);
			if(status != k_BRS_Failure) {
				m_LastBehavior = i;
				return status;
			}
		}
		return k_BRS_Failure;
	}

	virtual void on_terminate(NPC *npc) {
		m_LastBehavior = -1;
		m_CurrentBehavior.uninstall();
	};

private:
	int			 m_LastBehavior;
	Behavior m_CurrentBehavior;
};

class CompositeNode_Selector : public Composite_Node {
public:
	virtual AI_Task* create_task() {
		return new Selector_Task(this);
	}

	virtual void destroy_task(AI_Task* pTask) {
		Selector_Task* pTest = dynamic_cast<Selector_Task*>(pTask);
		//D_CHECK(pTest);
		D_SafeDelete(pTest);
	}
};

///  顺序节点：顺序执行子节点，如果返回finish就一直执行下个节点
class Sequence_Task : public AI_Task {
public:
	Sequence_Task(AI_Node* pNode)
		: AI_Task(pNode)
		, m_iCurrentChild(0)
	{}

	Composite_Node& get_composite_node() {
		return *dynamic_cast<Composite_Node*>(m_pNode);
	}

	virtual void on_init(NPC *npc) {}

	virtual Bev_Running_Status on_update(NPC *npc) {
		Composite_Node& comNode = get_composite_node();
		if(comNode.get_child_count() == 0)
			return k_BRS_Failure;;

		if(!m_CurrentBehavior.has_installed()) {
			m_iCurrentChild = 0;
			m_CurrentBehavior.install(*(comNode.get_child(m_iCurrentChild)));
		}
		while(true) {
			Bev_Running_Status status = m_CurrentBehavior.update(npc);
			if(status == k_BRS_SUCCESS) {
				m_iCurrentChild++;
				if(m_iCurrentChild >= comNode.get_child_count()) {
					return status;
				}
				m_CurrentBehavior.install(*(comNode.get_child(m_iCurrentChild)));
			} else {
				return status;
			}
		}
		return k_BRS_SUCCESS;
	}

	virtual void on_terminate(NPC *npc) {
		m_CurrentBehavior.uninstall();
	};

private:
	int m_iCurrentChild;
	Behavior m_CurrentBehavior;
};

class CompositeNode_Sequence : public Composite_Node {
public:
	virtual AI_Task* create_task() {
		return new Sequence_Task(this);
	}

	virtual void destroy_task(AI_Task* pTask) {
		Sequence_Task* pTest = dynamic_cast<Sequence_Task*>(pTask);
		//D_CHECK(pTest);
		D_SafeDelete(pTest);
	}
};

/// 同时进行,如呼吸动作
class Parallel_Task : public AI_Task {
public:
	Parallel_Task(AI_Node* pNode) : AI_Task(pNode) {
		Composite_Node& comNode = get_composite_node();
		for(int i = 0; i < comNode.get_child_count(); ++i) {
			m_Behaviors.push_back(new Behavior());
		}
	}
	virtual ~Parallel_Task() {
		for(size_t i = 0; i < m_Behaviors.size(); ++i) {
			D_SafeDelete(m_Behaviors[i]);
		}
	}

	Composite_Node& get_composite_node(){
		return *dynamic_cast<Composite_Node*>(m_pNode);
	}

	virtual void on_init(NPC *npc) {
		Composite_Node& comNode = get_composite_node();
		//D_CHECK(comNode.GetChildCount() == (int)m_Behaviors.size());
		for(size_t i = 0; i < m_Behaviors.size(); ++i) {
			m_Behaviors[i]->install(*(comNode.get_child(i)));
		}
	}

	virtual Bev_Running_Status on_update(NPC *npc) {
		//todo, or
		Bev_Running_Status status = k_BRS_SUCCESS;
		for(size_t i = 0; i < m_Behaviors.size(); ++i) {
			Bev_Running_Status ret = m_Behaviors[i]->update(npc);
			if(ret == k_BRS_Failure) {
				status = k_BRS_Failure;
			}
		}
		return status;
	}

	virtual void on_terminate(NPC *npc) {}

private:
	std::vector<Behavior*> m_Behaviors;
};

class CompositeNode_Parallel : public Composite_Node {
public:
	virtual AI_Task* create_task() {
		return new Parallel_Task(this);
	}

	virtual void destroy_task(AI_Task* pTask) {
		Parallel_Task* pTest = dynamic_cast<Parallel_Task*>(pTask);
		//D_CHECK(pTest);
		D_SafeDelete(pTest);
	}
};

#define DEF_TERMINATE_NODE(name, task) \
	class AI_Node##_##name : public AI_Node {\
	public:\
		virtual AI_Task* create_task(){\
			return new task(this);\
		}\
		virtual void destroy_task(AI_Task* pTask){ \
			task* pTest = dynamic_cast<task*>(pTask);\
			D_SafeDelete(pTest);	\
		};\
	};

#define CREATE_TERMINATE_NODE(name) new AI_Node##_##name()

#define CREATE_AI_ROOT_NODE(ai_id) \
	function create_ai_root_node (void) {\
		Composite_Node* pselect_ai_id = new CompositeNode_Selector();\
		return pselect_ai_id;\
	}

#endif /* BEHAVIOR_TREE_H_ */
