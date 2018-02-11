The Scope class can be used by any container that will store sets of variables

Class Template:
		parentScope should be the global scope, or the parent class template for inheritence
		isStatic should be true

Class Instance:
		parentScope should be the template for the class
		isStatic should be false

Generic Scope:
		parentScope should be the global scope, or the scope this was created in
		isStatic should be false
		-When a child scope is destroyed, parent.overwriteVariables(child) should be called so parent variables are updated
