import sys
import ast

def is_walkable(node):
	return isinstance(node, ast.BinOp) or isinstance(node, ast.BoolOp) or isinstance(node, ast.Compare) or isinstance(node, ast.UnaryOp)

def get_right(node):
	if isinstance(node, ast.UnaryOp) and isinstance(node.op, ast.USub):
		return node.operand
	try:
		return node.right
	except:
		pass
	try:
		if len(node.comparators) > 1:
			return None
		return node.comparators[0]
	except:
		pass
	return None

def get_left(node):
	if isinstance(node, ast.UnaryOp) and isinstance(node.op, ast.USub):
		return ast.Constant(-1)
	return node.left

def is_leaf(node):
	if isinstance(node, ast.UnaryOp):
		return False
	return 'left' not in node.__dict__ and 'right' not in node.__dict__

def postorder_walk(node, action, instruction_list, expr):
	if node is None:
		raise ValueError(f"Parsing fail:\n{expr}\nTry adding parentheses!")
	if isinstance(node, ast.BoolOp):
		for item in node.values:
			postorder_walk(item, action, instruction_list, expr)
	if is_leaf(node):
		action(node, instruction_list)
		return
	postorder_walk(get_left(node), action, instruction_list, expr)
	postorder_walk(get_right(node), action, instruction_list, expr)
	action(node, instruction_list)
