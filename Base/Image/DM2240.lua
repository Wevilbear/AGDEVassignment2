function SaveToLuaFile(outputString, overwrite)
   print("SaveToLuaFile...")
   local f;						-- The file
   if overwrite == 1 then		-- Wipe the contents with new data
      f = assert(io.open("Image/DM2240_HighScore.lua", "w"))
   elseif overwrite == 0 then	-- Append with new data
      f = assert(io.open("Image/DM2240_HighScore.lua", "a"))
   end
   -- Write to the file
   f:write(outputString)
   -- Close the file
   f:close()
   print("OK")
end

title = "DM2240 - Week 14 Scripting"
width = 800
height = 600
PlayerPos_x = 100
PlayerPos_z = 200

-- Keyboard Inputs
moveForward = "W"
moveBackward = "S"
moveLeft = "A"
moveRight = "D"

--CPlayerInfo start position
CPlayerInfoStartPos = {0,0,10}

function CalculateDistanceSquare(x1,y1,z1,x2,y2,z2)
	local distanceSquare = (x2 - x1)*(x2 - x1) + (y2 - y1)*(y2 - y1)+(z2 - z1)*(z2 - z1)
	print (distanceSquare)
	return distanceSquare
end