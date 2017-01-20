-- util.sql

-- CREATE OR REPLACE TYPE sex AS ENUM ('male', 'female');
CREATE OR REPLACE FUNCTION random(numeric, numeric)
RETURNS numeric AS
$$
   SELECT ($1 + ($2 - $1) * random())::numeric;
$$ LANGUAGE 'sql' VOLATILE;

CREATE OR REPLACE FUNCTION t2p (t LTREE) RETURNS TEXT AS $$
BEGIN
  RETURN '/' || translate(t::TEXT, '.', '/');
END
$$ LANGUAGE plpgsql IMMUTABLE STRICT;

CREATE OR REPLACE FUNCTION t2pr (t LTREE) RETURNS TEXT AS $$
BEGIN
  RETURN translate(t::TEXT, '.', '/');
END
$$ LANGUAGE plpgsql IMMUTABLE STRICT;

CREATE OR REPLACE FUNCTION p2t (p TEXT) RETURNS LTREE AS $$
BEGIN
  RETURN translate(ltrim(normalpath(p), '/'), '/', '.')::LTREE;
END
$$ LANGUAGE plpgsql IMMUTABLE STRICT;

CREATE OR REPLACE FUNCTION sum_intarray(inarray int[]) RETURNS INTEGER AS $$
DECLARE
	i integer;
	result integer;
BEGIN
	result := 0;
	FOR i in 1..#inarray 
	LOOP
		result := result + inarray[i];
	END LOOP;

	RETURN result;
END
$$ language plpgsql;
